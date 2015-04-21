#ifndef CCGRID_H_H
#define CCGRID_H_H
class Ccgrid{
public:
	vector<vector<vector<int> > > grid;//记录每个grid上都有哪些model
	float width,height;//网格的长宽
	float w,h;//一格的长宽
	float cx;//网格中心x坐标
	float cz;//网格中心z坐标 
	vector<int> clist;//潜在碰撞列表
	vector<int> iList;//iList和jList共同记录grid上哪些(i,j)位置的列表非空，为的是加速清空过程
	vector<int> jList;
	int top_iList_jList;//iList的栈顶指针，也是jList上的栈顶指针
	Ccgrid(){
		width=0;
		height=0;
		w=0;
		h=0;
		cx=0;
		cz=0;
	}
	void setFrame(float _width,float _height,float _w,float _h){//设置网格框架
		width=_width;
		height=_height;
		w=_w;
		h=_h;
		//
		int m=height/h+1;//heigth/h是最大行标，故height/h+1是行数
	    int n=width/w+1;
		grid.resize(m);
		for(int i=0;i<m;i++)grid[i].resize(n);
		//初始化iList和jList
		iList.resize(m);
		jList.resize(n);
	}
	void reserve_clist(int N){
		clist.reserve(N);
	
	}
	void setCenter(float _cx,float _cz){//设置网格中心
		cx=_cx;
		cz=_cz;
	}
	void clearContent(){

		//根据iList，jList，top_iList_jList清空grid
		for(int i=0;i<top_iList_jList;i++){
			grid[iList[i]][jList[i]].clear();
		}
		//清空iList和jList，注意，只是top_iList_jList置为0，不要动iList和jList
		top_iList_jList=0;
/*
		//检查一下grid是否真的清空了
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
	void fillContent(const vector<Cmodel*>&pmodelList){//填充网格内容
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
			if(_i>=0&&_i<m_grid&&_j>=0&&_j<n_grid){//要保证下标不越界(虽然理论上不应越界，但由于计算误差，还是有可能越界)
				grid[_i][_j].push_back(i);
				//存入iList和jList
				iList[top_iList_jList]=_i;
				jList[top_iList_jList]=_j;
				top_iList_jList++;
			}
		}//得到grid
	}

	void getclist(float x,float z){//获得(x,z)所在格的碰撞列表存入clist
		clist.clear();//清空clist
		//求(x,z)所在格的下标(_i,_j)
		int _x=x-cx+width/2;
		int _z=z-cz+height/2;
		int _i=_z/h;
		int _j=_x/w;
		//获得第(_i,_j)格及周围8个格中所有点的列表
		getlist(_i,_j);
	}
	void getlist(int _i,int _j){//获得第(i,j)格及周围8个格中所有点的列表存入clist
		int m=grid.size();
		int n=grid[0].size();
		if(!(_i>=0&&_i<m&&_j>=0&&_j<n))return;//需保证(_i,_j)不越界
		//中
		{
			const vector<int>&list_center=grid[_i][_j];
			int n=(int)list_center.size();
			for(int k=0;k<n;k++){
				clist.push_back(list_center[k]);
			}
		}
		//上
		if(_i-1>=0){
			const vector<int>&list_up=grid[_i-1][_j];
			int n=(int)list_up.size();
			for(int k=0;k<n;k++){
				clist.push_back(list_up[k]);
			}
		}
		//下
		if(_i+1<m){
			const vector<int>&list_dn=grid[_i+1][_j];
			int n=(int)list_dn.size();
			for(int k=0;k<n;k++){
				clist.push_back(list_dn[k]);
			}
		}
		//左
		if(_j-1>=0){
			const vector<int>&list_lt=grid[_i][_j-1];
			int n=(int)list_lt.size();
			for(int k=0;k<n;k++){
				clist.push_back(list_lt[k]);
			}
		}
		//右
		if(_j+1<n){
			const vector<int>&list_rt=grid[_i][_j+1];
			int n=(int)list_rt.size();
			for(int k=0;k<n;k++){
				clist.push_back(list_rt[k]);
			}
		}
		//左上
		if(_i-1>=0&&_j-1>=0){
			const vector<int>&list_uplt=grid[_i-1][_j-1];
			int n=(int)list_uplt.size();
			for(int k=0;k<n;k++){
				clist.push_back(list_uplt[k]);
			}
		}
		//左下
		if(_i+1<m&&_j-1>=0){
			const vector<int>&list_dnlt=grid[_i+1][_j-1];
			int n=(int)list_dnlt.size();
			for(int k=0;k<n;k++){
				clist.push_back(list_dnlt[k]);
			}
		}
		//右上
		if(_i-1>=0&&_j+1<n){
			const vector<int>&list_uprt=grid[_i-1][_j+1];
			int n=(int)list_uprt.size();
			for(int k=0;k<n;k++){
				clist.push_back(list_uprt[k]);
			}
		}
		//右下
		if(_i+1<m&&_j+1<n){
			const vector<int>&list_dnrt=grid[_i+1][_j+1];
			int n=(int)list_dnrt.size();
			for(int k=0;k<n;k++){
				clist.push_back(list_dnrt[k]);
			}
		}//clist做好
	}

};
#endif