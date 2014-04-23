#include <stdarg.h>
#include <string.h>

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

/**
	Fast log(num)/log(10)
	
	@param num .
		the value to calculate
	@return
		the successful value
*/
int log10s(int num)
{
	int res=0;
	while(num>=10)
	{
		num=(num/10);
		res++;
	}
	return res;
}

/**
	Converts int to string
	
	@param num .
		The value to convert
	@return NEW
		returns pointer to the new string
*/
char* b_int_to_string(int num)
{
	int isNegative=0;
	if(num<0)
	{
		isNegative=1;
		num=-num;
	}
	int i=0,length=log10s(num);
	i=length;
	char* endS=(char*)malloc((length+2+isNegative)*8);
	endS[i+1+isNegative]='\0';
	if(isNegative)
	{
		endS[0]='-';
	}
	while(0<=i)
	{
		endS[i+isNegative]='0'+num%10;
		num=(int)(num/10);
		i--;
	}
	return endS;
}