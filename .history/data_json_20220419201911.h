
#ifndef __DATA_JSON__h
#define __DATA_JSON__h
class Data_json{
public:
char *data;
int len;
Data_json(char *data,int len){
this->data = data;
this->len = len;
}
Data_json(){
this->data = NULL;
this->len = 0;
}
~Data_json(){
    //!!!!!加上delete产生了 defalut segment的bug
    delete data;
}
};
#endif