#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//get the args in the query_string, add them and return the value
void math_cgi(char *query_string)
{
	if(NULL == query_string)
		return ;
	char *data1 = NULL; //point to the arg1
	char *data2 = NULL; //point to the arg2

	while(*query_string != '\0')
	{
		if(*query_string == '=' && !data1)  //val1=a&val2=b
		{
			++query_string;
			data1 = query_string;
			continue;
		}
		if(*query_string == '&')
		{
			*query_string = '\0';
			++query_string;
		}
		if(*query_string=='=' && data1!=NULL )	//get arg2
		{
			//a=1&b=2
			++query_string;
			data2 = query_string;
			break;
		}
		++query_string;
	}
	int val1 = atoi(data1);
	int val2 = atoi(data2);
	int ret = val1 + val2;
	printf("<p>math : %d + %d == %d</p>\n", val1, val2, ret);
}


int main()
{
	int content_length = -1;
	char method[1024];
	char query_string[1024];
	char post_data[1024];

	bzero(method, sizeof(method));
	bzero(query_string, sizeof(query_string));
	bzero(post_data, sizeof(post_data));

	printf("<html>\n");
	printf("<head>MATH</head>\n");
	printf("<body>\n");
	
	//get query method
	strcpy(method, getenv("REQUEST_METHOD"));
	if(strcasecmp("GET", method) == 0) //GET
	{
		//get query_string
		strcpy(query_string, getenv("QUERY_STRING"));
		math_cgi(query_string);
	}
	else if(strcasecmp("POST", method) == 0) //POST
	{
		content_length = atoi(getenv("CONTENT_LENGTH"));
		//get POST data
		int i = 0;
		for(; i < content_length; ++i)
		{	
			read(0, &post_data[i], 1);
		}
		post_data[i] = '\0';
		math_cgi(post_data);
	}
	else
	{ 
		return 1;
	}

	printf("</body>\n");
	printf("</html>\n");
	
	return 0;
}
