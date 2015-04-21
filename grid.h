#ifndef CCGRID_H_H
#define CCGRID_H_H
class Ccgrid{
public:
	vector<vector<vector<int> > > grid;//��¼ÿ��grid�϶�����Щmodel
	float width,height;//����ĳ���
	float w,h;//һ��ĳ���
	float cx;//��������x����
	float cz;//��������z���� 
	vector<int> clist;//Ǳ����ײ�б�
	vector<int> iList;//iList��jList��ͬ��¼grid����Щ(i,j)λ�õ��б�ǿգ�Ϊ���Ǽ�����չ���
	vector<int> jList;
	int top_iList_jList;//iList��ջ��ָ�룬Ҳ��jList�ϵ�ջ��ָ��
	Ccgrid(){
		width=0;
		height=0;
		w=0;
		h=0;
		cx=0;
		cz=0;
	}
	void setFrame(float _width,float _height,float _w,float _h){//����������
		width=_width;
		height=_height;
		w=_w;
		h=_h;
		//
		int m=height/h+1;//heigth/h������б꣬��height/h+1������
	    int n=width/w+1;
		grid.resize(m);
		for(int i=0;i<m;i++)grid[i].resize(n);
		//��ʼ��iList��jList
		iList.resize(m);
		jList.resize(n);
	}
	void reserve_clist(int N){
		clist.reserve(N);
	
	}
	void setCenter(float _cx,float _cz){//������������
		cx=_cx;
		cz=_cz;
	}
	void clearContent(){

		//����iList��jList��top_iList_jList���grid
		for(int i=0;i<top_iList_jList;i++){
			grid[iList[i]][jList[i]].clear();
		}
		//���iList��jList��ע�⣬ֻ��top_iList_jList��Ϊ0����Ҫ��iList��jList
		top_iList_jList=0;
/*
		//���һ��grid�Ƿ���������
		bool allcleared=true;
		int m=grid.size();
		int n=grid[0].size();
		for(int i=0;i<m;i++){
			for(int j=0;j<n;j++){
				int size=(int)grid[i][j].size();
				if(size!=0){
					allcleared=false;
					break;
				}
			}
			if(allcleared==false)break;
		}
		if(allcleared){
			cout<<"all cleared"<<endl;
		}else{
			cout<<"not all cleared!"<<endl;
			system("pause");
		}
		*/
	}
	void fillContent(const vector<Cmodel*>&pmodelList){//�����������
		float halfwidth_sub_cx=width/2-cx;
		float halfheight_sub_cz=height/2-cz;
		int x,z,_i,_j;
		int m_grid=(int)grid.size();
		int n_grid=(int)grid[0].size();
		int n=pmodelList.size();
		for(int i=0;i<n;i++){
			float pos[4];
			pmodelList[i]->getPosInFather(pos);
			x=pos[X]+halfwidth_sub_cx;
			z=pos[Z]+halfheight_sub_cz;
			_i=z/h;
			_j=x/w;
			if(_i>=0&&_i<m_grid&&_j>=0&&_j<n_grid){//Ҫ��֤�±겻Խ��(��Ȼ�����ϲ�ӦԽ�磬�����ڼ����������п���Խ��)
				grid[_i][_j].push_back(i);
				//����iList��jList
				iList[top_iList_jList]=_i;
				jList[top_iList_jList]=_j;
				top_iList_jList++;
			}
		}//�õ�grid
	}

	void getclist(float x,float z){//���(x,z)���ڸ����ײ�б����clist
		clist.clear();//���clist
		//��(x,z)���ڸ���±�(_i,_j)
		int _x=x-cx+width/2;
		int _z=z-cz+height/2;
		int _i=_z/h;
		int _j=_x/w;
		//��õ�(_i,_j)����Χ8���������е���б�
		getlist(_i,_j);
	}
	void getlist(int _i,int _j){//��õ�(i,j)����Χ8���������е���б����clist
		int m=grid.size();
		int n=grid[0].size();
		if(!(_i>=0&&_i<m&&_j>=0&&_j<n))return;//�豣֤(_i,_j)��Խ��
		//��
		{
			const vector<int>&list_center=grid[_i][_j];
			int n=(int)list_center.size();
			for(int k=0;k<n;k++){
				clist.push_back(list_center[k]);
			}
		}
		//��
		if(_i-1>=0){
			const vector<int>&list_up=grid[_i-1][_j];
			int n=(int)list_up.size();
			for(int k=0;k<n;k++){
				clist.push_back(list_up[k]);
			}
		}
		//��
		if(_i+1<m){
			const vector<int>&list_dn=grid[_i+1][_j];
			int n=(int)list_dn.size();
			for(int k=0;k<n;k++){
				clist.push_back(list_dn[k]);
			}
		}
		//��
		if(_j-1>=0){
			const vector<int>&list_lt=grid[_i][_j-1];
			int n=(int)list_lt.size();
			for(int k=0;k<n;k++){
				clist.push_back(list_lt[k]);
			}
		}
		//��
		if(_j+1<n){
			const vector<int>&list_rt=grid[_i][_j+1];
			int n=(int)list_rt.size();
			for(int k=0;k<n;k++){
				clist.push_back(list_rt[k]);
			}
		}
		//����
		if(_i-1>=0&&_j-1>=0){
			const vector<int>&list_uplt=grid[_i-1][_j-1];
			int n=(int)list_uplt.size();
			for(int k=0;k<n;k++){
				clist.push_back(list_uplt[k]);
			}
		}
		//����
		if(_i+1<m&&_j-1>=0){
			const vector<int>&list_dnlt=grid[_i+1][_j-1];
			int n=(int)list_dnlt.size();
			for(int k=0;k<n;k++){
				clist.push_back(list_dnlt[k]);
			}
		}
		//����
		if(_i-1>=0&&_j+1<n){
			const vector<int>&list_uprt=grid[_i-1][_j+1];
			int n=(int)list_uprt.size();
			for(int k=0;k<n;k++){
				clist.push_back(list_uprt[k]);
			}
		}
		//����
		if(_i+1<m&&_j+1<n){
			const vector<int>&list_dnrt=grid[_i+1][_j+1];
			int n=(int)list_dnrt.size();
			for(int k=0;k<n;k++){
				clist.push_back(list_dnrt[k]);
			}
		}//clist����
	}

};
#endif