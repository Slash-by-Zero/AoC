#ifndef CMDLINE_H
#define CMDLINE_H

/* This module takes the comand line arguments and parses them so that options can be given
 * with <--key=value> without any mandated order of these arguments. Options are also suplemented
 * by a <basename(argv[0])>.conf file wich will be overloaded if the same option is also set on call.
 * */

/* Initializes this module for furthere use.
 * The contents of argv ar gonna be modified here.
 * From the first argument thats not of type <--key=value> onwards 
 * the arguments can be accessed with getRemainingArguments()
 * @return value: 0 on succes, -1 otherewise and errno is set
 * */
int cmdlineInit(int argc, char *argv[]);

/* The following behaviours only apply if cmdlineInit() was called prior.
 * */

/* This function will always return.
 * */
char *const getProgramName(void);

/* This function behaves self explanatory if the key exists.
 * Otherwise it returns NULL.
 * */
char *const getValueByKey(const char *key); 

/* This function returns the remaining arguments as described in cmdlineInit()
 * @param count: The address where the count of the remaining arguments is to be stored.
 * */
char **getRemainingArguments(int *count);
#endif
