
void updateLODlevelAndPointOrLineSize(){//����LOD�ȼ�����߿��
	float mycellpos[4];
	pcell->getPosInFather(mycellpos);
	//--------------------------���ݾ������ģ�ͷֱ���----------------------
	//water
	if(water.visible){
		float waterpos[4];
		water.getPosInFather(waterpos);
		waterpos[Y]=water.Horizon;
		//����water�����ǵ�α����
		float fakeDis_3d=fabs(mycellpos[X]-waterpos[X])+fabs(mycellpos[Y]-waterpos[Y])+fabs(mycellpos[Z]-waterpos[Z]);
		if(fakeDis_3d>50.0){
			water.tooFar=true;
		}else{
			water.tooFar=false;
		}
	}	//	cout<<water.tooFar<<endl;
	//superburst��LOD��pointSize
	{
		int nsuperburst=(int)psuperburstList.size();
		for(int i=0;i<nsuperburst;i++){
			Csuperburst&superburst=*(Csuperburst*)psuperburstList[i];
			if(superburst.isdied||superburst.visible==false)continue;
			float superburstPos[4];
			superburst.getPosInFather(superburstPos);
			float d_2=pow2(superburstPos[X]-frustum.campos[X])+pow2(superburstPos[Z]-frustum.campos[Z]);
			float d=CarmSqrt(d_2);
			//����doLOD	
			if(d>=400.0){
				superburst.LODlevel=1;
			}else if(d>=250.0){
				superburst.LODlevel=0;
			}else{
				superburst.LODlevel=0;
			}
			//����superburst��pointSize
			if(d<50.0){
				float d1,s1,d2,s2;//��d=d1ʱsize=s1,��d=d2ʱsize=s2
				d1=50.0;
				s1=30;
				d2=10.0;
				s2=260;//150
				float a,b;//size=a*d+b�е�a,b��������
				b=(s1*d2-s2*d1)/(d2-d1);
				a=(s1-b)/(d1+0.0001);//��Ӧ(s1-b)/d1��d1��1��Ŀ���Ƿ�ֹ���
				superburst.pointSize=min(56,a*d+b);
			}else if(d>200.0){
				float d1,s1,d2,s2;//��d=d1ʱsize=s1,��d=d2ʱsize=s2
				d1=200.0;
				s1=10;
				d2=500.0;
				s2=6;
				float a,b;//size=a*d+b�е�a,b��������
				b=(s1*d2-s2*d1)/(d2-d1);
				a=(s1-b)/(d1+0.0001);//��Ӧ(s1-b)/d1��d1��1��Ŀ���Ƿ�ֹ���
				superburst.pointSize=max(1,a*d+b);//Ҫ��֤����С��С��1���ܵ���Ч�������緢ɢ��
			}else{//��500<=d<=2000ʱ
				float d1,s1,d2,s2;//��d=d1ʱsize=s1,��d=d2ʱsize=s2
				d1=200.0;
				s1=10;
				d2=50.0;
				s2=30;
				float a,b;//size=a*d+b�е�a,b��������
				b=(s1*d2-s2*d1)/(d2-d1);
				a=(s1-b)/(d1+0.0001);//��Ӧ(s1-b)/d1��d1��1��Ŀ���Ƿ�ֹ���
				superburst.pointSize=a*d+b;
			}
		}
	}
	//------------------���ݾ���ѡ��ģ�Ͱ汾-------------------
	//����������Щ���廹δ�������ϸ�ڰ汾��ģ�ͣ�����Ŀǰδʵ��LOD
	//tree
	
	//autoCell

	//cannonball

	//amac

	//ene

	//gene

	//protein

}