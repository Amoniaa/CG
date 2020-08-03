
// CGMITRayTraceDoc.h : CCGMITRayTraceDoc ��Ľӿ�
//


#pragma once


class CCGMITRayTraceDoc : public CDocument
{
protected: // �������л�����
	CCGMITRayTraceDoc();
	DECLARE_DYNCREATE(CCGMITRayTraceDoc)

// ����
public:
	int m_methodSelect;
	float m_runTime;
	int sampleNum;
	int threadNum;

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// ʵ��
public:
	virtual ~CCGMITRayTraceDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// ����Ϊ����������������������ݵ� Helper ����
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
