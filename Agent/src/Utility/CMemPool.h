#pragma once
#include <cassert>
#include "XiamiRuntime.h"
#include "CLock.h"
#include <stdlib.h>

__MyNameSpaceBegin

template<size_t T,typename LOCK = CLock,size_t C = 16> 
class CMemPool
{ 
public: 
	CMemPool(); 
	virtual ~CMemPool(); 
    
	typedef CMemPool<T,LOCK,C> self_type;
	typedef LOCK               lock_type;
    
public: 
	void*   Alloc(); 
	void    Free(void *p); 
    
protected: 
	CMemPool(const self_type &Pool); 
	self_type operator =(const self_type &Pool);    
protected: 
	typedef struct tagNODE 
	{ 
		tagNODE *       pPre; 
		char            szBuf[T]; 
	}NODE, *PNODE; 
    
	typedef struct tagBLOCK 
	{
		tagBLOCK *      pPre;
		NODE            pNodes[C];
	}BLOCK, *PBLOCK; 
    
protected: 
	PBLOCK       _pBlocks; 
	PNODE        _pFreeNodes; 
    lock_type    _lock;
}; 


template<size_t T,typename LOCK,size_t C> 
inline CMemPool<T,LOCK,C>::CMemPool() 
: _pBlocks(NULL) 
, _pFreeNodes(NULL) 
{ 
} 

template<size_t T,typename LOCK,size_t C> 
inline CMemPool<T,LOCK,C>::~CMemPool() 
{ 
	_lock.Lock(); 
	while (NULL != _pBlocks) 
	{ 
		PBLOCK pItem = _pBlocks; 
		_pBlocks = _pBlocks->pPre; 
		//free(pItem);
		delete pItem;
	} 
	_lock.Unlock(); 
} 

template<size_t T,typename LOCK,size_t C> 
inline void* CMemPool<T,LOCK,C>::Alloc() 
{ 
	_lock.Lock(); 
    
	if (_pFreeNodes == NULL) 
	{ 
       // PBLOCK pBlock = (PBLOCK)malloc(sizeof(BLOCK)); 
		PBLOCK pBlock = new BLOCK;
		assert(pBlock);
		if (NULL == pBlock ) 
		{ 
			_lock.Unlock(); 
			return NULL; 
		} 
        
		pBlock->pPre = _pBlocks; 
		pBlock->pNodes[0].pPre = NULL; 
		for (size_t i=1;i<C;++i)  
		{ 
			pBlock->pNodes[i].pPre = &pBlock->pNodes[i - 1]; 
		} 
		_pBlocks = pBlock; 
		_pFreeNodes = &_pBlocks->pNodes[C - 1]; 
	}
    
	PNODE pNode = _pFreeNodes; 
	_pFreeNodes = _pFreeNodes->pPre; 
    
	_lock.Unlock(); 
	
    //pNode->pPre = (PNODE)T; //指向一个常指针区域
	pNode->pPre = (PNODE)this;

	return pNode->szBuf; 
} 

template<size_t T,typename LOCK,size_t C> 
inline void CMemPool<T,LOCK,C>::Free(void *p) 
{ 
	PNODE pNode = (PNODE)((char*)p - sizeof(PNODE)); 
    
	_lock.Lock(); 
	pNode->pPre = _pFreeNodes; 
	_pFreeNodes = pNode; 
	_lock.Unlock(); 
} 

template<typename LOCK,size_t C> 
class CMemPool<0,LOCK,C>; //MAKE ERROR


/*******************************************************************************/
class CMemPoolBase {
public:
    CMemPoolBase(){}
    virtual ~CMemPoolBase(){}
    
public:
    virtual void* Alloc(size_t size)=0;
    virtual void  Free(void* p)=0;
};


template <typename LOCK = CLock>
class CMemPoolMgr : public CMemPoolBase
{
public:
	explicit CMemPoolMgr();
	virtual ~CMemPoolMgr();
public:
	void* Alloc(size_t size);
	void  Free(void* p);
    
protected: 
	typedef CMemPoolMgr<LOCK> self_type;
	typedef LOCK    lock_type;
	CMemPoolMgr(const self_type &Pool); 
	self_type operator =(const self_type &Pool);
    
protected:
	typedef struct tagNODE {
		tagNODE *  pPre;
		tagNODE *  pNext;
		size_t     size;
	}NODE,*PNODE;
    
protected:
	PNODE            _pHeader;
	lock_type        _lock;
    
    CMemPool<16,lock_type>     _pool16; 
	CMemPool<32,lock_type>     _pool32; 
	CMemPool<64,lock_type>     _pool64; 
	CMemPool<128,lock_type>    _pool128; 
	CMemPool<256,lock_type>    _pool256; 
	CMemPool<512,lock_type>    _pool512; 
	CMemPool<1024,lock_type>   _pool1024;
	CMemPool<2048,lock_type>   _pool2048;
	CMemPool<4096,lock_type>   _pool4096;
	CMemPool<8192,lock_type>   _pool8192;
};

template <typename LOCK>
inline CMemPoolMgr<LOCK>::CMemPoolMgr()
:_pHeader(NULL)
{

}

template <typename LOCK>
inline CMemPoolMgr<LOCK>::~CMemPoolMgr()
{
	_lock.Lock();
	while(NULL != _pHeader)
	{
		PNODE pItem = _pHeader;
		_pHeader    = _pHeader->pPre;
		free(pItem);
	}
	_lock.Unlock();
}

template <typename LOCK>
inline void* CMemPoolMgr<LOCK>::Alloc(size_t size)
{
	assert(size > 0);
    
	if (size <= 0)
		return NULL;
    
	if ( size <= 16 ) 
		return _pool16.Alloc(); 
    
	if ( size <= 32 ) 
		return _pool32.Alloc(); 
    
	if ( size <= 64 ) 
		return _pool64.Alloc(); 
    
	if ( size <= 128 ) 
		return _pool128.Alloc(); 
    
	if ( size <= 256 ) 
		return _pool256.Alloc(); 
    
	if ( size <= 512 ) 
		return _pool512.Alloc(); 
    
	if ( size <= 1024 ) 
		return _pool1024.Alloc(); 
    
	if (size <= 2048)
		return _pool2048.Alloc();
    
	if (size <= 4096)
		return _pool4096.Alloc();
    
	if (size <= 8192)
		return _pool8192.Alloc();
    
    
	size_t newsize = size + sizeof(NODE);
    newsize =  (((newsize)+7) &~ 7);// 按8字节对齐
    
	PNODE pItem = (PNODE)malloc(newsize);
    
	if(NULL == pItem)
		return NULL;
    
	_lock.Lock();
	pItem->pPre       = _pHeader;
    if(NULL != _pHeader)  (_pHeader->pNext = pItem);
	_pHeader          = pItem;
	_lock.Unlock();

	pItem->size        = (size_t)this;
     

  //  printf("alloc pitem:%p,this:%p\n",pItem,this);
	return (char*)pItem + sizeof(NODE);
}

template <typename LOCK>
inline void CMemPoolMgr<LOCK>::Free(void* p)
{
	assert(NULL != p);
	if (NULL == p ) 
		return ; 
    
    
	size_t size = *((size_t*)p - 1); 
	assert(size > 0);
    
    if (this == (self_type *)size) {
        
        PNODE pItem = (PNODE)((char*)p - sizeof(NODE));
     //   printf("free  pitem:%p,this:%p\n",pItem,this);

        _lock.Lock();
        if(pItem == _pHeader)
            _pHeader = _pHeader->pPre;
        else {
            PNODE preNode = pItem->pPre;
            pItem->pNext->pPre = preNode;
            if(preNode)
                preNode->pNext = pItem->pNext;
        }
        _lock.Unlock();
        
        free(pItem);
        return;
    }
    
    CMemPool<16,LOCK> * pool = (CMemPool<16,LOCK> *)size;  
    return pool->Free(p);
}

__MyNameSpaceEnd

typedef __MyNameSpace(CMemPoolMgr<__MyNameSpace(CNullLock)>) CScopePool;
