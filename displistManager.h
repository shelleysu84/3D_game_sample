#ifndef CDISPLISTMANAGER_H_H
#define CDISPLISTMANAGER_H_H
class CdisplistManager{
public:
	vector<GLuint> displistList;
	~CdisplistManager(){
		destory();
	}
	GLuint genDisplist(){
		GLuint displist=glGenLists(1);
		if(displist==0){
			cout<<"glGenListsÊ§°Ü!"<<endl;
			system("pause");
		}
		displistList.push_back(displist);
		return displist;
	}
	void destory(){
		int nDispList=(int)displistList.size();
		for(int i=0;i<nDispList;i++){
			if(displistList[i]!=0)glDeleteLists(displistList[i],1);
		}
		//Çå¿ÕÁÐ±í
		displistList.clear();
	}

};
#endif