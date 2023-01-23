#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <libgen.h>
#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <fcntl.h>
#include <glob.h>
#include <sys/wait.h>

#include "cmdline.h"

static void die(char *msg){
	perror(msg);
	exit(EXIT_FAILURE);
}

static void printFile(FILE *toPrint, FILE *stream){
	rewind(toPrint);
	int c=fgetc(toPrint);
	while(c != EOF){
		c = fputc(c, stream);
		if(c == EOF) die("fputc");
		c = fgetc(toPrint);
	}
	if(ferror(toPrint)){
		die("fgetc");
	}
	
	fprintf(stream, "\n");
}

struct pathResults{
	char *path;
	struct programResults *results;
	struct pathResults *next;
};

struct programResults{
	char *user;
	char *fileName;
	int complete, exitcode;
	long count, total, max, min;
	struct programResults* next;
};


static FILE *shell;
static long runs=1024, avg_delay=0, time_limit=0;
static char *cwd;
static int printOutput=1;

static void testPaths(struct pathResults *);

static void testProgram(struct programResults *, char *);

struct tabularStyle{
	char *hdash1, *hdash2, *vdash, *hvdash1, *hvdash2;
	int hline, all;
};

static struct tabularStyle style_utf16 = {
	.hline=1,
	.hdash1 = "\u2500", .hdash2 = "\u2550", .vdash = "\u2502",
	.hvdash1 = "\u253c", .hvdash2 = "\u256a"
	},
	style_utf8 = {
		.hline = 1,
		.hdash1 = "-", .hdash2 = "=", .vdash = "|",
		.hvdash1 = "+", .hvdash2 = "‡"
	}, 
	style_csv ={
		.hline = 0, .all=1,
		.vdash = ";"
	};

static void printResults(struct pathResults *, struct tabularStyle *);

int main(int argc, char *argv[]){
	if(cmdlineInit(argc, argv) < 0) die("cmdlineInit");
	char *path;
	
	{ //Read path either from --path or generate the path for --day
		path = getValueByKey("day");
		
		if(!path){
			path=getValueByKey("path");
			
			if(!path){
				fprintf(stderr, "Either day or path option must be given!\n");
				exit(EXIT_FAILURE);
			}
		}
		else{
			long day=0;
			if(strcmp(path, "all") != 0){
				errno=0;
				day = strtol(path, NULL, 0);
				if(errno!=0) die("strtol");
				
				if(day < 1 || day > 25){
					fprintf(stderr, "Day option must be between 1 and 25! Given day: %ld\n", day);
					exit(EXIT_FAILURE);
				}
			}
			
			long year=2015;
			
			char *tmp=getValueByKey("year");
			
			time_t rawtime;
			time(&rawtime);
			
			struct tm *gmt = gmtime(&rawtime);
			if(!gmt) die("gmtime");
			
			if(tmp){
				errno=0;
				year=strtol(tmp, NULL, 0);
				if(errno != 0) die("strtol");
				
				if(year < 2015 || year > 1900 + gmt->tm_year){
					fprintf(stderr, "Year option must be between 2015 and %d! Given year: %ld\n", gmt->tm_year, year);
					exit(EXIT_FAILURE);
				}
			}
			else{
				if(!gmt) die("gmtime");
				
				year = 1900 + (gmt->tm_mon == 11 ? gmt->tm_year : (gmt->tm_year - 1));
			}
			
			path=calloc(14 + (size_t) log10((double) year), 1);
			if(!path) die("calloc");
			if(day != 0) sprintf(path, "%ld/%02ld/*.out", year, day);
			else sprintf(path, "%ld/*/*.out", year);
		}
	}
	
	char *user;
	struct tabularStyle *style = &style_utf16;
	
	{ //Handling of all other options
		user=getValueByKey("user");
		if(!user){
			user=".";
		}
		char *tmp = getValueByKey("runs");
		if(tmp){
			errno = 0;
			runs = strtol(tmp, NULL, 0);
			if(errno != 0) die("strtol");
			
			if(runs < 1){
				fprintf(stderr, "runs must be a positive long! Given runs: %ld\n", runs);
				exit(EXIT_FAILURE);
			}
		}
		tmp = getValueByKey("tablstyle");
		if(tmp){
			if(strcmp(tmp, "utf8") == 0){
				style = &style_utf8;
			}
			else if(strcmp(tmp, "csv") == 0){
				style = &style_csv;
			}
			else if(strcmp(tmp, "utf16") == 0){
				style = &style_utf16;
			}
		}
		tmp = getValueByKey("print-output");
		if(tmp && strcmp(tmp, "no") == 0){
			printOutput = 0;
		}
		else if(tmp && strcmp(tmp, "yes") != 0){
			fprintf(stderr, "print-output must be yes or no!\n");
			exit(EXIT_FAILURE);
		}
		tmp = getValueByKey("time-limit");
		if(tmp){
			errno = 0;
			time_limit = strtol(tmp, NULL, 0);
			if(errno != 0) die("strtol");
			
			if(time_limit < 0 || time_limit >= LONG_MAX/1000000){
				fprintf(stderr, "time-limit must be a positive long! Given time-limite: %s\n", tmp);
				exit(EXIT_FAILURE);
			}
			
			time_limit *= 1000000;
		}
		tmp = getValueByKey("avg-delay");
		if(tmp){
			errno = 0;
			avg_delay = strtol(tmp, NULL, 0);
			if(errno != 0) die("strtol");
			if(avg_delay < 0){
				fprintf(stderr, "avg-delay must be a positive long!\n");
				exit(EXIT_FAILURE);
			}
		}
	}
	
	{ //Setting up file descriptors for upcoming testing!
		int stdout_cpy=dup(fileno(stdout)); //save stdout to another FILE* for output of this program
		if(stdout_cpy < 0) die("dup");
		shell = fdopen(stdout_cpy, "w");
		if(!shell) die("fdopen");
		
		int oldflags = fcntl(stdout_cpy, F_GETFD, 0); //Making sure other programms dont have the shell as fd (stderr should be sued for debugging messages)
		if(oldflags < 0) die("fcntl");
		oldflags |= FD_CLOEXEC;
		if(fcntl(stdout_cpy, F_SETFD, oldflags) < 0) die("fcntl");
	}
	
	int cwd_length=128;
	cwd=malloc(128);
	if(!cwd) die("malloc");
	while(errno = 0, !getcwd(cwd,cwd_length)){
		if(errno != ERANGE) die("getcwd");
		cwd=realloc(cwd, cwd_length = cwd_length << 1);
		if(!cwd) die("realloc");
	}
	
	struct pathResults *first=NULL;
	glob_t globbuf;
	
	{
		if(glob(user, 0, NULL, &globbuf) != 0){
			int tmp = errno;
			fprintf(stderr, "%s/%s: ", cwd, user);
			errno = tmp;
			die("glob");
		}
		
		int total = 0;
		
		for(int i=0;i<globbuf.gl_pathc;i++){
			char *user_tmp = globbuf.gl_pathv[i];
			char path_full[strlen(user_tmp)+1+strlen(path)+1];
			glob_t globbuf2;
			sprintf(path_full, "%s/%s", user_tmp, path);
			int rVal = glob(path_full, 0, NULL, &globbuf2);
			if(rVal != 0 && rVal != GLOB_NOMATCH){
				int tmp = errno;
				fprintf(stderr, "%s/%s", cwd, path_full);
				errno = tmp;
				die("glob");
			}
			
			total += globbuf2.gl_pathc;
			
			for(int k=0;k<globbuf2.gl_pathc;k++){
				char *path_tmp = globbuf2.gl_pathv[k]+strlen(user_tmp)+1;
				char *name_tmp = strdup(basename(path_tmp));
				if(!name_tmp) die("strdup");
				path_tmp = dirname(path_tmp);
				
				for(struct pathResults **walk=&first; ; walk = &((*walk)->next)){
					if(!*walk){
						*walk = malloc(sizeof(**walk));
						if(!*walk) die("malloc");
						
						(*walk)->path=strdup(path_tmp);
						if(!(*walk)->path) die("strdup");
						(*walk)->results = calloc(1,sizeof(*((*walk)->results)));
						if(!(*walk)->results) die("calloc");
						(*walk)->results->user = user_tmp;
						(*walk)->results->fileName = name_tmp;
						(*walk)->next=NULL;
						break;
					}
					
					int cmp=strcmp(path_tmp, (*walk)->path);
					if(cmp == 0){
						struct programResults *new = calloc(1,sizeof(*new));
						if(!new) die("calloc");
						
						new->user = user_tmp;
						new->fileName = name_tmp;
						new->next = (*walk)->results;
						(*walk)->results = new;
						break;
					}
					if(cmp < 0){
						struct pathResults *new= malloc(sizeof(*new));
						if(!new) die("malloc");
						
						new->path = strdup(path_tmp);
						if(!new->path) die("strdup");
						new->results = calloc(1,sizeof(*((*walk)->results)));
						if(!new->results) die("calloc");
						new->results->user = user_tmp;
						new->results->fileName = name_tmp;
						new->next=*walk;
						*walk=new;
						break;
					}
				}
			}
			globfree(&globbuf2);
		}
		
		if(total == 0){
			fprintf(stderr, "No matching files found for! %s/%s/%s\n", cwd, user, path);
			exit(EXIT_FAILURE);
		}
	}
	
	testPaths(first);
	
	printResults(first, style);
	
	globfree(&globbuf);
	free(cwd);
	{
		char tmp;
		if(&tmp > path) free(path); //Check if path is on heap if yes free
	}
}

static void testPaths(struct pathResults *res){
	if(!res->results){
		fprintf(stderr, "No programms found for path: %s\n", res->path);
		return;
	}
	
	{//Redirect stdin to input file of first target program 
		char dirc[strlen(cwd)+strlen(res->results[0].user)+strlen(res->path)+3];
		if(sprintf(dirc, "%s/%s/%s", cwd, res->results[0].user, res->path) < 0) die("sprintf");
		char input[strlen(dirc)+11];
		if(sprintf(input, "%s/input.txt", dirc) < 0) die("sprintf");
		
		if(!freopen(input, "r", stdin)){
			int terrno = errno;
			fprintf(stderr, "%s/%s/%s/input.txt: ", cwd, res->results[0].user, res->path);
			errno = terrno;
			die("freopen");
		}
	}
	
	for(struct programResults *walk=res->results;walk;walk=walk->next){
		testProgram(walk, res->path);
	}
	
	if(res->next) testPaths(res->next);
}

static void testProgram(struct programResults *res, char *path){
	char *execname;
	{
		//chdir to directory of target program and get execname
		char dirc[strlen(cwd)+strlen(res->user)+strlen(path)+4];
		sprintf(dirc, "%s/%s/%s/", cwd, res->user, path);
		
		if(chdir(dirc) < 0){
			fprintf(stderr, "%s: ", dirc);
			die("chdir");
		}
		
		execname = res->fileName;
	}
	
	
	//TESTING! Testing. testing ...
	struct timeval start, end;
	res->total=0;
	res->max = 0;
	res->min = LONG_MAX;
	res->exitcode = 0;
	res->complete = 0;
	FILE *buf, *first_buf=NULL;
	int exit_code;
	
	for(res->count = 0; res->count < runs;){
		buf=tmpfile(); //create tmpfile for the program output
		
		int oldflags = fcntl(fileno(buf), F_GETFD, 0); //Making sure buf will only be accessable through stdout
		if(oldflags < 0) die("fcntl");
		oldflags |= FD_CLOEXEC;
		if(fcntl(fileno(buf), F_SETFD, oldflags) < 0) die("fcntl");
		
		rewind(stdin);
		
		if(!buf) die("tmpfile");
		if(dup2(fileno(buf), fileno(stdout)) < -1) die("dup2"); //redirect stdout to the tmpfile
		
		// Start of section no unneccessary code between here and the end of it!
		if(gettimeofday(&start, NULL) != 0) die("gettimeofday");
		
		pid_t pid=fork();
		if(pid < 0) die("fork");
		
		if(pid == 0){
			execl(execname, execname, NULL);
			int terrno = errno;
			fprintf(stderr, "%s/%s/%s: ", res->user, path, res->fileName);
			errno = terrno;
			die("execl");
		}
		
		waitpid(pid, &exit_code, 0); //Wait for program to finish execution
		
		if(gettimeofday(&end, NULL) != 0) die("gettimeofday"); 
		// End of timed section
		
		res->count++;
		
		if(!WIFEXITED(exit_code)){ //Check if program terminated normally
			fprintf(stderr, "Program %s/%s/%s did not finish execution! Further tests wont be necessary!\n\n", res->user, path, res->fileName);
			
			if(fclose(buf) != 0) die("fclose");
			if(res->count != 1 && fclose(first_buf) != 0) die("fclose");
			
			return;
		}
		else if(WEXITSTATUS(exit_code) != 0){
			fprintf(stderr, "Forked process returned an unexpected exit status on program %s/%s/%s: %d\n\n", res->user, path, res->fileName, WEXITSTATUS(exit_code));
			res->exitcode = WEXITSTATUS(exit_code);
			
			if(fclose(buf) != 0) die("fclose");
			if(res->count != 1 && fclose(first_buf) != 0) die("fclose");
			
			return;
		}
		
		
		long runtime =
			(long)(end.tv_sec - start.tv_sec) * 1000000 +
			(long)(end.tv_usec - start.tv_usec) - avg_delay;
		
		res->total += runtime;
		if(runtime > res->max) res->max = runtime;
		if(runtime < res->min) res->min = runtime;
		
		rewind(buf);
		
		if(first_buf){
			rewind(first_buf);
			
			int c_buf=fgetc(buf), c_cmp=fgetc(first_buf);
			
			while(c_buf == c_cmp && c_buf != EOF){
				c_buf = fgetc(buf);
				if(ferror(buf)) die("fgetc");
				c_cmp = fgetc(first_buf);
			}
			if(ferror(first_buf)) die("fgetc");
			if(c_buf != c_cmp){
				fprintf(stderr, "Programm %s/%s/%s produced a different output on run %ld, despite being given the same input!\n\n", res->user, path, res->fileName, res->count);
				fprintf(stderr, "This output:\n");
				printFile(buf, stderr);
				
				fprintf(stderr, "Expected output:\n");
				printFile(first_buf, stderr);
				
				if(fclose(buf) != 0) die("fclose");
				
				return;
			}
			
			if(fclose(buf) != 0) die("fclose");
		}
		else{
			first_buf=buf; //Saving output of first program to compare the output of the res-> against it
			
			int oldflags = fcntl(fileno(first_buf), F_GETFD, 0); //Making sure the programs cant read the output of the first program
			if(oldflags < 0) die("fcntl");
			oldflags |= FD_CLOEXEC;
			fcntl(fileno(first_buf), F_SETFD, oldflags);
		}
		
		if(time_limit != 0 && res->total >= time_limit) break;
	}
	
	if(printOutput){
		fprintf(shell, "Output of %s/%s/%s:\n", res->user, path, res->fileName);
		printFile(first_buf, shell);
	}
	res->complete = 1;
	
	if(fclose(first_buf) != 0) die("fclose");
	
}

static char *convTime(long time, int length){
	static int buf_len = 0;
	static char *buf = NULL;
	if(((int) log10(time)) + 6 > buf_len){
		buf_len=((int) log10(time)) + 6;
		buf = realloc(buf, buf_len);
		if(!buf) die("realloc");
	}
	
	if(time < 1000){
		sprintf(buf, "%*ld µs", length-3, time);
	}
	else if(time < 1000000){
		sprintf(buf, "%*ld.%02ld ms", length-6, time/1000, (time%1000)/10);
	}
	else if(time < 60000000){
		sprintf(buf, "%*ld.%03ld s", length-6, time/1000000, (time%1000000)/1000);
	}
	else{
		sprintf(buf, "%*ld:%02ld.%03ld m", length-9, time/60000000, (time%60000000)/1000000, (time%1000000)/1000);
	}
	
	return buf;
}

void printResults(struct pathResults *results, struct tabularStyle *style){
	if(!results || !results->results) exit(2);
	
	int max_path=4, max_user=4, max_time=1, max_fileName=4;
	
	for(struct pathResults *walk=results; walk ; walk=walk->next){
		if(strlen(walk->path) > max_path) max_path = strlen(walk->path);
		for(struct programResults *walk2=walk->results; walk2; walk2=walk2->next){
			if(strlen(walk2->user) > max_user) max_user = strlen(walk2->user);
			if(strlen(walk2->fileName) > max_fileName) max_fileName = strlen(walk2->fileName);
			if((int) log10(walk2->total) > max_time) max_time = (int) log10(walk2->total);
		}
	}
	
	max_time+=4;
	int hline_length=max_path+max_user+max_fileName+((int) log10(runs))+4*max_time+34;
	
	fprintf(shell, "%-*s %s ", max_path, "PATH", style->vdash);
	fprintf(shell, "%-*s %s ", max_user, "USER", style->vdash);
	fprintf(shell, "%-*s %s ", max_fileName, "FILE", style->vdash);
	fprintf(shell, "%-*s %s ", (int) log10(runs) + 1, "RUNS", style->vdash);
	fprintf(shell, "%-9s %s ", "COMPLETED", style->vdash);
	fprintf(shell, "%-*s %s ", max_time, "TOTAL", style->vdash);
	fprintf(shell, "%-*s %s ", max_time, "AVG", style->vdash);
	fprintf(shell, "%-*s %s ", max_time, "MAX", style->vdash);
	fprintf(shell, "%-*s\n", max_time, "MIN");
	
	int ref[8];
	ref[0] = max_path + 1;
	ref[1] = ref[0] + max_user + 3;
	ref[2] = ref[1] + max_fileName + 3;
	ref[3] = ref[2] + log10(runs) + 4;
	ref[4] = ref[3] + 12;
	for(int i=5;i<8;i++){
		ref[i] = ref[i-1] + max_time + 3;
	}
	
	if(style->hline) {
		for(int i=0;i<hline_length;i++) {
			for(int j = 0;j<8;j++){
				if(ref[j] == i){
					fprintf(shell, "%s", style->hvdash2);
					i++;
				}
			}
			fprintf(shell, "%s", style->hdash2);
		}
		fprintf(shell, "\n");
	}
	
	for(struct pathResults *walk=results; walk ;){
		if(walk != results && style->hline){
			for(int i=0;i<hline_length;i++){
				for(int j=0;j<8;j++){
					if(ref[j] == i){
						fprintf(shell, "%s", style->hvdash1);
						i++;
					}
				}
				fprintf(shell, "%s", style->hdash1); 
			}
			fputc('\n', shell);
		}
		for(struct programResults *walk2=walk->results; walk2;){
			fprintf(shell, "%*s %s ", max_path, (walk2 == walk->results || style->all) ? walk->path : "", style->vdash);
			fprintf(shell, "%*s %s ", max_user, walk2->user, style->vdash);
			fprintf(shell, "%*s %s ", max_fileName, walk2->fileName, style->vdash);
			fprintf(shell, "%*ld %s ", (int) log10(runs) + 1, walk2->count, style->vdash);
			fprintf(shell, "%9s %s ", walk2->complete ? "yes" : "no", style->vdash);
			fprintf(shell, "%s %s ", convTime(walk2->total, max_time), style->vdash);
			fprintf(shell, "%s %s ", convTime(walk2->total/walk2->count, max_time), style->vdash);
			fprintf(shell, "%s %s ", convTime(walk2->max, max_time), style->vdash);
			fprintf(shell, "%s\n", convTime(walk2->min == LONG_MAX ? 0 : walk2->min, max_time));
			struct programResults *tmp=walk2->next;
			free(walk2->fileName);
			free(walk2);
			walk2=tmp;
		}
		struct pathResults *tmp=walk->next;
		free(walk->path);
		free(walk);
		walk=tmp;
	}
	
}
