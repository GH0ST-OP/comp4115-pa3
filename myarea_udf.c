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

   #include <math.h>

   my_bool myarea_udf_init(UDF_INIT *initid, UDF_ARGS *args, char *message);
   void myarea_udf_deinit(UDF_INIT *initid __attribute__((unused)));
   double myarea_udf(UDF_INIT* initid, UDF_ARGS* args __attribute__((unused)),
                     char* is_null __attribute__((unused)), char* error __attribute__((unused)));

   my_bool myarea_udf_init(UDF_INIT *initid, UDF_ARGS *args, char *message)
   {
     if(!(args->arg_count == 1)) {
       strcpy(message, "Expected one argument (real)");
       return 1;
     }

     args->arg_type[0] = REAL_RESULT;

     return 0;
   }

   void myarea_udf_deinit(UDF_INIT *initid __attribute__((unused)))
   {

   }

   double myarea_udf(UDF_INIT* initid, UDF_ARGS* args __attribute__((unused)),
                     char* is_null __attribute__((unused)), char* error __attribute__((unused)))
   {
     double radius = *((double *)(args->args[0]));
     double area = M_PI*radius*radius;

     return area;
   }

   #endif /* HAVE_DLOPEN */
