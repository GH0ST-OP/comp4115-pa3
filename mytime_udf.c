#ifdef STANDARD
   /* STANDARD is defined, don't use any mysql functions */
   #include <stdlib.h>
   #include <stdio.h>
   #include <string.h>
   #ifdef __WIN__
   typedef unsigned __int64 ulonglong;/* Microsofts 64 bit types */
   typedef __int64 longlong;
   #else
   typedef unsigned long long ulonglong;
   typedef long long longlong;
   #endif /*__WIN__*/
   #else
   #include <my_global.h>
   #include <my_sys.h>
   #if defined(MYSQL_SERVER)
   #include <m_string.h>/* To get strmov() */
   #else
   /* when compiled as standalone */
   #include <string.h>
   #define strmov(a,b) stpcpy(a,b)
   #define bzero(a,b) memset(a,0,b)
   #define memcpy_fixed(a,b,c) memcpy(a,b,c)
   #endif
   #endif
   #include <mysql.h>
   #include <ctype.h>

   #ifdef HAVE_DLOPEN

   #if !defined(HAVE_GETHOSTBYADDR_R) || !defined(HAVE_SOLARIS_STYLE_GETHOST)
   static pthread_mutex_t LOCK_hostname;
   #endif

   my_bool mytime_udf_init(UDF_INIT *initid, UDF_ARGS *args, char *message);
   void mytime_udf_deinit(UDF_INIT *initid __attribute__((unused)));
   char* mytime_udf(UDF_INIT* initid, UDF_ARGS* args, char* result,
                      unsigned long* length, char* is_null, char* error);
   my_bool mytime_udf_init(UDF_INIT *initid, UDF_ARGS *args, char *message)
   {
     if(!(args->arg_count == 1)) {
       strcpy(message, "Expected one argument (INT)");
       return 1;
     }

     args->arg_type[0] = INT_RESULT;

     return 0;
   }

   void mytime_udf_deinit(UDF_INIT *initid __attribute__((unused)))
   {

   }

   char* mytime_udf(UDF_INIT* initid, UDF_ARGS* args, char* result,
                      unsigned long* length, char* is_null, char* error)
   {
     char str[1024];
     longlong sec = *((longlong*)(args->args[0]));
     int hr, min, t;
     hr = sec/3600;
     t = sec%3600;
     min = t/60;
     sec = t%60;

     snprintf(str, 1024, "%d:%d:%lld", hr, min, sec);

     *length = strlen(str);
     memcpy(result, str, strlen(str));
     return result;
   }

   #endif /* HAVE_DLOPEN */
