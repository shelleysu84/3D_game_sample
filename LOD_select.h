
void updateLODlevelAndPointOrLineSize(){//更新LOD等级或点线宽度
	float mycellpos[4];
	pcell->getPosInFather(mycellpos);
	//--------------------------根据距离调整模型分辨率----------------------
	//water
	if(water.visible){
		float waterpos[4];
		water.getPosInFather(waterpos);
		waterpos[Y]=water.Horizon;
		//计算water与主角的伪距离
		float fakeDis_3d=fabs(mycellpos[X]-waterpos[X])+fabs(mycellpos[Y]-waterpos[Y])+fabs(mycellpos[Z]-waterpos[Z]);
		if(fakeDis_3d>50.0){
			water.tooFar=true;
		}else{
			water.tooFar=false;
		}
	}	//	cout<<water.tooFar<<endl;
	//superburst的LOD和pointSize
	{
		int nsuperburst=(int)psuperburstList.size();
		for(int i=0;i<nsuperburst;i++){
			Csuperburst&superburst=*(Csuperburst*)psuperburstList[i];
			if(superburst.isdied||superburst.visible==false)continue;
			float superburstPos[4];
			superburst.getPosInFather(superburstPos);
			float d_2=pow2(superburstPos[X]-frustum.campos[X])+pow2(superburstPos[Z]-frustum.campos[Z]);
			float d=CarmSqrt(d_2);
			//设置doLOD	
			if(d>=400.0){
				superburst.LODlevel=1;
			}else if(d>=250.0){
				superburst.LODlevel=0;
			}else{
				superburst.LODlevel=0;
			}
			//设置superburst的pointSize
			if(d<50.0){
				float d1,s1,d2,s2;//当d=d1时size=s1,当d=d2时size=s2
				d1=50.0;
				s1=30;
				d2=10.0;
				s2=260;//150
				float a,b;//size=a*d+b中的a,b两个参数
				b=(s1*d2-s2*d1)/(d2-d1);
				a=(s1-b)/(d1+0.0001);//本应(s1-b)/d1，d1加1的目的是防止零除
				superburst.pointSize=min(56,a*d+b);
			}else if(d>200.0){
				float d1,s1,d2,s2;//当d=d1时size=s1,当d=d2时size=s2
				d1=200.0;
				s1=10;
				d2=500.0;
				s2=6;
				float a,b;//size=a*d+b中的a,b两个参数
				b=(s1*d2-s2*d1)/(d2-d1);
				a=(s1-b)/(d1+0.0001);//本应(s1-b)/d1，d1加1的目的是防止零除
				superburst.pointSize=max(1,a*d+b);//要保证不过小，小于1可能导致效果错误（如发散）
			}else{//当500<=d<=2000时
				float d1,s1,d2,s2;//当d=d1时size=s1,当d=d2时size=s2
				d1=200.0;
				s1=10;
				d2=50.0;
				s2=30;
				float a,b;//size=a*d+b中的a,b两个参数
				b=(s1*d2-s2*d1)/(d2-d1);
				a=(s1-b)/(d1+0.0001);//本应(s1-b)/d1，d1加1的目的是防止零除
				superburst.pointSize=a*d+b;
			}
		}
	}
	//------------------根据距离选择模型版本-------------------
	//由于以下这些物体还未制作多个细节版本的模型，所以目前未实现LOD
	//tree
	
	//autoCell

	//cannonball

	//amac

	//ene

	//gene

	//protein

}