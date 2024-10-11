/* This function converts a string to a double 
   1) if the return value of the function is 0 then 
      the string is not a double (it contains characters that
      could not be converted into a double) 
   2)  if the return value is one the conversion to a double 
       has been successful, and the double value is placed in
	   the variable in the calling program pointed to by the 
	   pointer result (the second argument)  */
int str_to_double(const char *str, double *result) {
	char *endp;
	double converted;

	converted = strtod(str, &endp);
	if (*endp != '\0' || endp == str) {
		return 0;
	}
	*result = converted;
	return 1;
}


/* This function converts a string to a integer
   1) if the return value of the function is 0 then
	  the string is not an integer (it contains characters that
	  could not be converted into an integer)
   2)  if the return value is one the conversion to an integer
	   has been successful, and the integer value is placed in
	   the variable in the calling program pointed to by the
	   pointer result (the second argument)  */
int str_to_integer(const char *str, long int *result) {
	char *endp;
	long int converted;

	converted = strtol(str, &endp, 10);
	if (*endp != '\0' || endp == str) {
		return 0;
	}
	*result = converted;
	return 1;
}
