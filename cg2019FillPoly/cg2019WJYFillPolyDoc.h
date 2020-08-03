
// cg2019WJYFillPolyDoc.h: Ccg2019WJYFillPolyDoc 类的接口
//


#pragma once


class Ccg2019WJYFillPolyDoc : public CDocument
{
protected: // 仅从序列化创建
	Ccg2019WJYFillPolyDoc() noexcept;
	DECLARE_DYNCREATE(Ccg2019WJYFillPolyDoc)

// 特性
public:
	int m_displayMode = -1;
	int m_drawMode = 0;

// 操作
public:

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 实现
public:
	virtual ~Ccg2019WJYFillPolyDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
