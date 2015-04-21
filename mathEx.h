//-----------------------------��ѧ����
//ע�⣺�ض���������ִ���3ά�����ĺ�����ֻ��2ά��4ά��Ҳ����˵��3dͼ��ѧ���һ��Ҫ�ž�ʹ��3ά������
//���������debug������release�������������ԭ�����ʹ���˴���4ά�����ĺ���ȥ����3ά����
//linline���������������ʵ�ֶ�д��.h�ļ���
inline float fastsin(float x) 
//��������x�ǻ���ֵ��x��[-3pi,3pi]֮��ʱЧ�����
{	
	//always wrap input angle to -PI..PI

	if(x>=-9.42477795&&x<=9.42477795){
		if (x< -3.14159265)
			x+= 6.28318531;
		else if(x>  3.14159265)
			x-= 6.28318531;
	}else{
		int k=floor(0.5-x/6.28318531);
		x+=6.28318531*k;
	
	}

	//compute sine
	float rs;
	if (x< 0)
	{
		rs= 1.27323954 * x+ .405284735 * x* x;
		
		if (rs< 0)
			rs= .225 * (rs*-rs- rs) + rs;
		else
			rs= .225 * (rs* rs- rs) + rs;
	}
	else
	{
		rs= 1.27323954 * x- 0.405284735 * x* x;
		
		if (rs< 0)
			rs= .225 * (rs*-rs- rs) + rs;
		else
			rs= .225 * (rs* rs- rs) + rs;
	}


	//return
	return rs;
}


inline float fastcos(float x){ 
	//always wrap input angle to -PI..PI
	if(x>=-9.42477795&&x<=9.42477795){
		if (x< -3.14159265)
			x+= 6.28318531;
		else if(x>  3.14159265)
			x-= 6.28318531;
	}else{
		int k=floor(0.5-x/6.28318531);
		x+=6.28318531*k;
	
	}
	//compute cosine: sin(x + PI/2) = cos(x)
	x+= 1.57079632;
	if (x>  3.14159265)
		x-= 6.28318531;
	float rs;
	if (x< 0)
	{
		rs= 1.27323954 * x+ 0.405284735 * x* x;
		
		if (rs< 0)
			rs= .225 * (rs*-rs- rs) + rs;
		else
			rs= .225 * (rs* rs- rs) + rs;
	}
	else
	{
		rs= 1.27323954 * x- 0.405284735 * x* x;
		
		if (rs< 0)
			rs= .225 * (rs*-rs- rs) + rs;
		else
			rs= .225 * (rs* rs- rs) + rs;
	}
	
	//return
	return rs;


}


inline float CarmSqrt(float x)
{
	union{//ע�⣬������union���Ա�֤intPart��floatPart���ڴ�ռ�������������
		int intPart;
		float floatPart;
	}cv1,cv2;
	cv1.floatPart = x;
	cv2.floatPart = x;
	cv1.intPart = 0x1FBCF800 + (cv1.intPart >> 1);
	cv2.intPart = 0x5f3759df - (cv2.intPart >> 1);
	return 0.5f*(cv1.floatPart + (x * cv2.floatPart));

};

inline void printv(float v[4]){
	cout<<v[X]<<" "<<v[Y]<<" "<<v[Z]<<endl;
}

inline int rem(int a,int b){//��"%"�����߼���ȡ������
	int r=a%b;
	if(r<0)return r+b;
	else return r;
}
inline void init4(float vec[4],float x,float y,float z,float w){
	vec[0]=x;
	vec[1]=y;
	vec[2]=z;
	vec[3]=w;
}
inline void init2(float v[2],float x,float y){
	v[0]=x;
	v[1]=y;
}

inline float getLen(const float v[4]){
	return CarmSqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
}
inline float getLen_2(const float v[4]){//����ģ��ƽ��
	return v[0]*v[0]+v[1]*v[1]+v[2]*v[2];
}
inline void sub(const float (&v1)[4],const float (&v2)[4],float (&vrs)[4]){
	vrs[0]=v1[0]-v2[0];
	vrs[1]=v1[1]-v2[1];
	vrs[2]=v1[2]-v2[2];
	vrs[3]=v1[3]-v2[3];
}
inline void add(const float v1[4],const float v2[4],float vrs[4]){
	vrs[0]=v1[0]+v2[0];
	vrs[1]=v1[1]+v2[1];
	vrs[2]=v1[2]+v2[2];
	vrs[3]=v1[3]+v2[3];

}
inline void addadd(const float v1[4],const float v2[4],const float v3[4],float vrs[4]){
	vrs[0]=v1[0]+v2[0]+v3[0];
	vrs[1]=v1[1]+v2[1]+v3[1];
	vrs[2]=v1[2]+v2[2]+v3[2];
	vrs[3]=v1[3]+v2[3]+v3[3];
}
inline float dot(const float v1[4],const float v2[4]){
	return v1[X]*v2[X]+v1[Y]*v2[Y]+v1[Z]*v2[Z];

}
inline void mul(float k,const float v[4],float vrs[4]){
	vrs[0]=v[0]*k;
	vrs[1]=v[1]*k;
	vrs[2]=v[2]*k;
	vrs[3]=v[3]*k;
}
inline void mulAdd(const float k,const float v1[4],const float v2[4],float vrs[4])
//vrs=k*v1+v2
//�˺����������壬��Ӧ�ִ�cpu�ܽ��˺ͼӺϲ���һ��ָ����ص�
//���Ժ��л��Ὣ��ǰû��ʹ�ô˺����Ĳ����𽥸ĳ�ʹ�ô˺���
{
	vrs[0]=k*v1[0]+v2[0];
	vrs[1]=k*v1[1]+v2[1];
	vrs[2]=k*v1[2]+v2[2];
	vrs[3]=k*v1[3]+v2[3];

}
inline void mul_mat(float k,float mat[16]){
	for(int i=0;i<=15;i++){
	mat[i]*=k;
	}
}

inline void add_mat(float a[16],float b[16],float rs[16]){
	for(int i=0;i<=15;i++){
		rs[i]=a[i]+b[i];
	}

}
inline float normalize_p(float v[4]){//��ȷ��λ��

    float r2=v[0]*v[0] + v[1]*v[1] + v[2]*v[2];
	if(r2==0.0)return 0;
	float r=sqrt(r2);
    v[0] /=r;
    v[1] /=r;
    v[2] /=r;
	return r;

}

inline float normalize(float v[4])//ǰ�˷���ģ��ƽ����v[W]������0��
{
 
	float r2=v[0]*v[0] + v[1]*v[1] + v[2]*v[2];
	if(r2==0.0)return 0;

	 float xhalf = 0.5f*r2;  
     int i = *(int*)&r2; // get bits for floating VALUE   
     i = //0x5f375a86
		 0x5f3759df - (i>>1); // gives initial guess y0  
     float x = *(float*)&i; // convert bits BACK to float  
     x = x*(1.5f-xhalf*x*x); // Newton step, repeating increases accuracy  
     //�õ�x=1.0/CarmSqrt(r2)
	 v[0]*=x;
	 v[1]*=x;
	 v[2]*=x;
	 return r2;
}

inline void matcopy( const float src[16],float tg[16])
{
	memcpy(tg,src,sizeof(float)*16);	
}
inline void veccopy(const float vec[4],float tgvec[4]){
	tgvec[X]=vec[X];
	tgvec[Y]=vec[Y];
	tgvec[Z]=vec[Z];
	tgvec[W]=vec[W];
	//�����memcpy����ᱨwarning: warning C4789: destination of memory copy is too small
	//��˻�����ڴ��ֹ�ģ��С�������ֱ�������ֵ����
}
inline float normalize(float v[4],float vrs[4])//ǰ�˷���ģ��ƽ����v[W]������0��
{

 
	veccopy(v,vrs);
	float r2=vrs[0]*vrs[0] + vrs[1]*vrs[1] + vrs[2]*vrs[2];
	if(r2==0.0)return 0;

	 float xhalf = 0.5f*r2;  
     int i = *(int*)&r2; // get bits for floating VALUE   
     i = //0x5f375a86
		 0x5f3759df - (i>>1); // gives initial guess y0  
     float x = *(float*)&i; // convert bits BACK to float  
     x = x*(1.5f-xhalf*x*x); // Newton step, repeating increases accuracy  
     //�õ�x=1.0/CarmSqrt(r2)
	 vrs[0]*=x;
	 vrs[1]*=x;
	 vrs[2]*=x;
	 return r2;
}
inline void cross(float v1[4], float v2[4], float result[4])
{
    result[0] = v1[1]*v2[2] - v1[2]*v2[1];
    result[1] = v1[2]*v2[0] - v1[0]*v2[2];
    result[2] = v1[0]*v2[1] - v1[1]*v2[0];
	result[3] = 0;//��Ϊ��˽���������������Ե���άΪ0
}
inline void makeE(float m[16])
{
	m[0] = 1; m[4] = 0; m[8] = 0; m[12] = 0;
    m[1] = 0; m[5] = 1; m[9] = 0; m[13] = 0;
    m[2] = 0; m[6] = 0; m[10] = 1; m[14] = 0;
    m[3] = 0; m[7] = 0; m[11] = 0; m[15] = 1;
}


inline void prodv_wOne(const float matrix[16], const float in[4],
		      float out[4]){//in��out������ͬһ����
	float _out[4];//��ʱ��ת
	_out[0]=in[0]*matrix[0]+in[1]*matrix[4]+in[2]*matrix[8]+matrix[12];
	_out[1]=in[0]*matrix[1]+in[1]*matrix[5]+in[2]*matrix[9]+matrix[13];
	_out[2]=in[0]*matrix[2]+in[1]*matrix[6]+in[2]*matrix[10]+matrix[14];
	_out[3]=in[0]*matrix[3]+in[1]*matrix[7]+in[2]*matrix[11]+matrix[15];
	out[0]=_out[0];
	out[1]=_out[1];
	out[2]=_out[2];
	out[3]=_out[3];
}
inline void prodv_lastRow0001_wOne(const float matrix[16],const float in[4],float out[4])
//in��out������ͬһ����
//����matrixΪ
// a d g x 
// b e h y 
// c f i z 
// 0 0 0 1  
//�;���
//in[W]=1
{
	float _out[4];//��ʱ��ת
	_out[0]=in[0]*matrix[0]+in[1]*matrix[4]+in[2]*matrix[8]+matrix[12];
	_out[1]=in[0]*matrix[1]+in[1]*matrix[5]+in[2]*matrix[9]+matrix[13];
	_out[2]=in[0]*matrix[2]+in[1]*matrix[6]+in[2]*matrix[10]+matrix[14];
	_out[3]=1;
	out[0]=_out[0];
	out[1]=_out[1];
	out[2]=_out[2];
	out[3]=_out[3];
}

inline void prodv_lastRow0001_wZero(const float matrix[16],const float in[4],float out[4])
//in��out������ͬһ����
//����matrixΪ
// a d g x 
// b e h y 
// c f i z 
// 0 0 0 1  
//�;���
//in[W]=0
{
	float _out[4];//��ʱ��ת
	_out[0]=in[0]*matrix[0]+in[1]*matrix[4]+in[2]*matrix[8];
	_out[1]=in[0]*matrix[1]+in[1]*matrix[5]+in[2]*matrix[9];
	_out[2]=in[0]*matrix[2]+in[1]*matrix[6]+in[2]*matrix[10];
	_out[3]=0;
	out[0]=_out[0];
	out[1]=_out[1];
	out[2]=_out[2];
	out[3]=_out[3];
}
inline void inv_RMmat_IOdiffer(const float RMmat[16],float rsmat[16])
//RM��������
//RMmat��rsmat������ͬһ������
{
	// a d g x        a d g 
	// b e h y   ���� b e h  �������������������Ϊ
	// c f i z        c f i
	// 0 0 0 1
	//
	// a b c -xa-yb-zc
	// d e f -xd-ye-zf
	// g h i -xg-yh-zi
	// 0 0 0 1
	//
	//����RMmat�������rsmat
	//rsmat��3*3����ΪRMmat��3*3���ֵ�ת��
	rsmat[0]=RMmat[0];
	rsmat[1]=RMmat[4];
	rsmat[2]=RMmat[8];
	rsmat[4]=RMmat[1];
	rsmat[5]=RMmat[5];
	rsmat[6]=RMmat[9];
	rsmat[8]=RMmat[2];
	rsmat[9]=RMmat[6];
	rsmat[10]=RMmat[10];
	//rsmat�����һ��Ϊ0 0 0 1
	rsmat[3]=0;
	rsmat[7]=0;
	rsmat[11]=0;
	rsmat[15]=1;
	//rsmat�����һ��ǰ������
	rsmat[12]=-RMmat[12]*RMmat[0]-RMmat[13]*RMmat[1]-RMmat[14]*RMmat[2];
	rsmat[13]=-RMmat[12]*RMmat[4]-RMmat[13]*RMmat[5]-RMmat[14]*RMmat[6];
	rsmat[14]=-RMmat[12]*RMmat[8]-RMmat[13]*RMmat[9]-RMmat[14]*RMmat[10];


}
inline void inv_RMmat(float RMmat[16])
//RM��������
//������ԭ��
{
	float rsmat[16];
	inv_RMmat_IOdiffer(RMmat,rsmat);
	matcopy(rsmat,RMmat);
}
inline int inv_general(const float src[16], float inverse[16])
//һ��4*4��������
//���߿�Ϊͬһ����
{
    int i, j, k, swap;
    float t;
    float temp[4][4];

    for (i=0; i<4; i++) {
	for (j=0; j<4; j++) {
	    temp[i][j] = src[i*4+j];
	}
    }
    makeE(inverse);

    for (i = 0; i < 4; i++) {
	/*
	** Look for largest element in column
	*/
	swap = i;
	for (j = i + 1; j < 4; j++) {
	    if (fabs(temp[j][i]) > fabs(temp[i][i])) {
		swap = j;
	    }
	}

	if (swap != i) {
	    /*
	    ** Swap rows.
	    */
	    for (k = 0; k < 4; k++) {
		t = temp[i][k];
		temp[i][k] = temp[swap][k];
		temp[swap][k] = t;

		t = inverse[i*4+k];
		inverse[i*4+k] = inverse[swap*4+k];
		inverse[swap*4+k] = t;
	    }
	}

	if (temp[i][i] == 0) {
	    /*
	    ** No non-zero pivot.  The matrix is singular, which shouldn't
	    ** happen.  This means the user gave us a bad matrix.
	    */
	    return GL_FALSE;
	}

	t = temp[i][i];
	for (k = 0; k < 4; k++) {
	    temp[i][k] /= t;
	    inverse[i*4+k] /= t;
	}
	for (j = 0; j < 4; j++) {
	    if (j != i) {
		t = temp[j][i];
		for (k = 0; k < 4; k++) {
		    temp[j][k] -= temp[i][k]*t;
		    inverse[j*4+k] -= inverse[i*4+k]*t;
		}
	    }
	}
    }
    return GL_TRUE;
}

inline float fastArccos(float x) { //���ټ���arccos  
	return (-0.69813170079773212 * x * x - 0.87266462599716477) * x + 1.5707963267948966;
 } 

inline float getA(float v1[4],float v2[4]){//��v1ת��v2�ĵ��ǣ�����acos���˽��ƹ�ʽ�������ǲ���ȷ�Ľ��
	//��v1��v2�ļн�
	float veclen=getLen(v2);
	float dirlen=getLen(v2);
	if(veclen==0||dirlen==0)return 0;
	float cosA=dot(v1,v2)/(veclen*dirlen);
	//���ڼ���������õ���cosAδ����[-1,1]֮��(��������ƫ��)����Ҫǿ�Ƶ��������
	//��һ������Ҫ�������������A=acos(cosA)ʱ�ὫA�õ�������ֵ1.#QNAN
	if(cosA>1){
		cosA=1;
	}else if(cosA<-1){
		cosA=-1;
	}
    float A=fastArccos(cosA);//acos(cosA);
	A=A*57.29;//180/pi=57.29
	//����ȷ��A�ķ���
	float norm[4];
	cross(v1,v2,norm);
	if(norm[Y]<0)A=-A;//�õ������ŵ�A
	return A;	
}
inline float pow2(float a){//ƽ��
	return a*a;
}
inline void orthogonalization33(float mat[16])
//��mat��3*3���ֽ��й淶������
//*****���˼�������⣬����ת�Ĳ�ֵ����Ҳ���ܻ�ʹ�����÷�����*****
//*****�����ܴ���ѧ��֤����������Ĳ�ֵ����Ӱ��������**************
{
//cout<<"hi"<<endl;
	//����<,>���ڻ���
	//
	//b1=a1                      u1=b1/|b1|
	//b2=a2-<a2,u1>u1            u2=b2/|b2|
	//b3=a3-<a3,u1>u1-<a3,u2>u2  u3=b3/|b3|
	//��b1,b2,b3Ϊ��������u1,u2,u3Ϊ��׼������
	//
	//���й淶������
	float a1[4]={mat[0],mat[1],mat[2],0};
	float a2[4]={mat[4],mat[5],mat[6],0};
	float a3[4]={mat[8],mat[9],mat[10],0};
	float u1[4],u2[4],u3[4];
	{
		float b1[4],b2[4],b3[4];
		//����b1,u1
		{
			veccopy(a1,b1);
			normalize(b1,u1);
		}
		//����b2,u2
		{
			float k=dot(a2,u1);
			float ku1[4];
			mul(k,u1,ku1);
			sub(a2,ku1,b2);
			normalize(b2,u2);
		}
		//����b3,u3
		{
			float k=dot(a3,u1);
			float _k=dot(a3,u2);
			float ku1[4],_ku2[4];
			mul(k,u1,ku1);
			mul(_k,u2,_ku2);
			float temp[4];//ku1+_ku2
			add(ku1,_ku2,temp);
			sub(a3,temp,b3);
			normalize(b3,u3);
		}
	}
	//�õ�u1,u2,u3
	//��u1~3������mat
	mat[0]=u1[0];mat[1]=u1[1];mat[2]=u1[2];
	mat[4]=u2[0];mat[5]=u2[1];mat[6]=u2[2];
	mat[8]=u3[0];mat[9]=u3[1];mat[10]=u3[2];
	
}


inline void neg(const float v[4],float vrs[4]){
	vrs[0]=-v[0];
	vrs[1]=-v[1];
	vrs[2]=-v[2];
	vrs[3]=-v[3];
}