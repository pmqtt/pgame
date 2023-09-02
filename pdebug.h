#ifndef PDEBUG_H
#define PDEBUG_H
#include <iostream>
#include <string>

#define P_UNUSED(x) (void)x;

class PPrinter{
    public:
        PPrinter() : _indent(0){}
        
        void indent(){
            _indent++;
        }

        void unindent(){
            _indent--;
            if (_indent < 0){
                _indent = 0;
            }
        }

        void reset()const{
            std::cout << "\033[0m";
        }

        void print_title(const std::string & str)const{
            reset();
            for(int i = 0; i < _indent; i++){
                std::cout << "\t";
            }
            //bold
            std::cout << "\033[1m";
            std::cout << str << std::endl;
        }

        void print_error(const std::string & str)const{
            reset();
            for(int i = 0; i < _indent; i++){
                std::cout << "\t";
            }
            std::cout << "\033[1;31m";
            std::cout << "ERROR: " << str << std::endl;
        }

        void print_ok(const std::string & str)const{
            reset();
            for(int i = 0; i < _indent; i++){
                std::cout << "\t";
            }
            std::cout << "\033[1;32m";
            std::cout << "OK: " << str << std::endl;
        }

        void print(const std::string & str)const{
            reset();
            for(int i = 0; i < _indent; i++){
                std::cout << "\t";
            }
            std::cout << str << std::endl;
        }

    private:
        int _indent;
};



#endif
