#pragma once

#include "TypeDefine.h"
#include "cg2019WJY3DTransDoc.h"
#include "cg2019WJY3DTransView.h"


class CDrawScene
{
public:
	CDrawScene();
	~CDrawScene();

	CDrawScene(Ccg2019WJY3DTransView* pView);

	void DrawScene();
	float xCenter, yCenter, zCenter;
private:
	int      m_objNumber;
	Object_p m_whoObject;           // Object pointer
	Ccg2019WJY3DTransView* m_pView;
	float glObjColor[SPACEOBJECTS][3];

	//ͨ��OpenGLʵ�ֳ������ƣ���������ά�ռ��X��Y��Z�����ᣬ���������ӵ㣬������Դ���˺�����CDrawScene::CDrawScene()�б�����
	void DrawBackground();

	//������ά�ռ䳡���е����壺���������ں���CDrawScene::CDrawScene()�б����ã�ͨ��forѭ�����ʵ��
	void DrawSpaceObject();

	//���Ʊ任�����άͼ�Σ��ں���CDrawScene::CDrawScene()�б�����
	void TransSpaceObject();

	//���ƾ����Ӽ��任�����ά�ռ伸���壬�ں���CDrawScene::CDrawScene()�б�����
	void projectSpaceObject();

	//���ڼ���任���󣬸��ݲ�ͬѡ�-X,+X,-Y,+Y,-Z,+Z,�Լ���ͬ�任ģʽ��translate��rotate�������Ӧ�ı任����ͨ����������������ذ�ť���ı䰴ť�Ĳ���ֵ���ں���CDrawScene::CDrawScene()�б����á�
	void CaculateMatrix();

	//��X����ת�任�������ں���CaculateMatrix()�б�����
	void rotateX3Dmatrix(float S, float C);

	//��Y����ת�任�������ں���CaculateMatrix()�б�����
	void rotateY3Dmatrix(float S, float C);

	//��Z����ת�任�������ں���CaculateMatrix()�б�����
	void rotateZ3Dmatrix(float S, float C);
	
	//ƽ�Ʊ任�������ں���CaculateMatrix()�б�����
	void translate3dMatrix(float dx, float dy, float dz);

	//ʵ�������ĺ������ں���CDrawScene::CDrawScene()�б�����
	void pRemoveBackFace();

	//���²ü��㷨��Ҫ���õĺ�������CDrawScene::pClipSpaceObject()������
	void pClipSpaceObject();
	int pVisible(float x, float y, float z, int pane);
	int outPut(float x, float y, float z, int *outCount, Gpoint_p polyClip);
	int pLineCrossPane(float sx, float sy, float sz,
		float px, float py, float pz, int pane);
	int pLineInterSectPane(float sx, float sy, float sz,
		float px, float py, float pz,
		int pane, int *outCount, Gpoint_p polyClip);
	
	void pObjectCenter();     //������������ĵ㣬��pLightSpaceBall()������
	void pLightSpaceBall();   //�ж���������������������ͶӰ����
	void pLightSpaceObject(); //�����������������������޷�ͶӰ����
};

