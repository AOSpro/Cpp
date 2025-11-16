#include "shaders.h"
std::string getFileContents(const char* filename){
    std::ifstream in(filename,std::ios::binary);
    if(in){
        std::string contents;
        in.seekg(0,std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0,std::ios::beg);
        in.read(&contents[0],contents.size());
        in.close();
        return(contents);
    }
    throw(errno);
}
Shader::Shader(const char* vfile,const char* ffile){
    std::string vcode=getFileContents(vfile);
    std::string fcode=getFileContents(ffile);
    const char* vsrc=vcode.c_str();
    const char* fsrc=fcode.c_str();
    GLuint vshader=glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vshader,1,&vsrc,NULL);
    glCompileShader(vshader);
    compileErrors(vshader,"VERTEX");
    GLuint fshader=glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fshader,1,&fsrc,NULL);
    glCompileShader(fshader);
    compileErrors(fshader,"FRAGMENT");
    ID = glCreateProgram();
    glAttachShader(ID,vshader);
    glAttachShader(ID,fshader);
    glLinkProgram(ID);
    compileErrors(ID,"PROGRAM");
    glDeleteShader(vshader);
    glDeleteShader(fshader);
}
void Shader::Activate(){
    glUseProgram(ID);
}
void Shader::Delete(){
    glDeleteProgram(ID);
}
void Shader::compileErrors(unsigned int shader,const char *type){
    GLint hasCompiled;
    char infoLog[1024];
    if(std::string(type)!="PROGRAM"){
        glGetShaderiv(shader,GL_COMPILE_STATUS,&hasCompiled);
        if(hasCompiled==GL_FALSE){
            glGetShaderInfoLog(shader,1024,NULL,infoLog);
        }
    }else{
        glGetProgramiv(shader,GL_LINK_STATUS,&hasCompiled);
        if(hasCompiled==GL_FALSE){
            glGetProgramInfoLog(shader,1024,NULL,infoLog);
        }
    }
}
