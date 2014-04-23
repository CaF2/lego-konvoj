/**
	Will make a new concentrated string, ( @param logical str+str+str+... )
	
	@param str NO_FREE
		the first string
	@param ... VA_LIST NULL_TERMINATED
		a va list with strings.
	@return
		the pointer to the new string
*/
char *concat(const char *str, ...)
{
	va_list ap, ap2;
	size_t total = 1;
	const char *s;
	char *result;
     
	va_start (ap, str);
	va_copy (ap2, ap);
     
	//Determine how much space we need. 
	for (s = str; s != NULL; s = va_arg (ap, const char *))
		total += strlen (s);
     
	va_end (ap);
     
	result = (char *) malloc (total);
	if (result != NULL)
	{
		result[0] = '\0';
     
		//Copy the strings.
		for (s = str; s != NULL; s = va_arg (ap2, const char *))
		strcat (result, s);
	}
     
	va_end (ap2);
     
	return result;
}