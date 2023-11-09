/*!\file
*/

#ifndef CPU_ERRORS_ENUM_H_INCLUDED
#define CPU_ERRORS_ENUM_H_INCLUDED


    #define RED "\033[0;31m"
    #define RST "\x1b[0m" 


    typedef enum cpu_errors{BAD_PROC_POINTER   = -1,  //
                            BAD_CODE_POINTER   = -2,  //
                            BAD_REGS_POINTER   = -3,  //
                            CUR_BIGGER_N_CMD   = -4,   //
                            BAD_STK_POINTER    = -5,
    } cpu_errors;

    static const char *cpu_error_names[] = {
        "All is ok",
        "Bad proc pointer",
        "Bad code pointer",
        "Bad regs pointer",
        "Curr cmd number bigger then amount of commands",    //4
        "Bad stack pointer",
    };

#endif
