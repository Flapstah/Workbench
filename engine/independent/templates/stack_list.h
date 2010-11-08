#if !defined(__STACK_LIST_H__)
#define __STACK_LIST_H__

//==============================================================================

namespace engine
{
	//============================================================================
	// TStackList
	//============================================================================
	template <typename _type>
	class TStackList
	{
	protected:
		typedef _type _element;

		class TIterator;

		//==========================================================================
		// TNode
		//==========================================================================
		class TNode
		{
		public:
			TNode(_element& element) : m_element(element) {}
			~TNode(void) {	Unlink();	}

			void Append(TNode& node)
			{
				node.Unlink();
				node.m_pPrevious = this;

				if (m_pNext != NULL)
				{
					m_pNext->m_pPrevious = &node;
					node.m_pNext = m_pNext;
				}

				m_pNext = &node;
			}

			void Prepend(TNode& node)
			{
				node.Unlink();
				node.m_pNext = this;

				if (m_pPrevious != NULL)
				{
					m_pPrevious->m_pNext = &node;
					node.m_pPrevious = m_pPrevious;
				}

				m_pPrevious = &node;
			}

			bool operator>(const TNode& rhs) const	{	return m_element > rhs.m_element;		}
			bool operator<(const TNode& rhs) const	{	return m_element < rhs.m_element;		}
			bool operator==(const TNode& rhs) const	{ return m_element == rhs.m_element;	}
			bool operator!=(const TNode& rhs) const	{ return m_element != rhs.m_element;	}

			operator _element&(void)								{ return m_element;										}

		protected:
			void Unlink(void)
			{
				if (m_pNext != NULL)			m_pNext->m_pPrevious = m_pPrevious;
				if (m_pPrevious != NULL)	m_pPrevious->m_pNext = m_pNext;
			}

			TNode*		m_pNext;
			TNode*		m_pPrevious;
			_element&	m_element;

		private:
			friend class TIterator;
			friend class TStackList;
		}; // End [class TNode]

		//==========================================================================

	public:

		//==========================================================================
		// TIterator
		//==========================================================================
		class TIterator
		{
		public:
			TIterator(TStackList& list) : m_list(list), m_pNode(list.Head()) {}
			~TIterator(void) { m_pNode = NULL; }

			TIterator& operator++(void)	{ if (m_pNode->m_pNext) m_pNode = m_pNode->m_pNext; } // prefix
			TIterator operator++(int)		{	TIterator ret = *this;	++(*this);	return ret;		} // postfix
			TIterator operator==(const TIterator& rhs)	{	return m_pNode == rhs.m_pNode;		}
			TIterator operator!=(const TIterator& rhs)	{	return m_pNode != rhs.m_pNode;		}

			operator TNode&(void)				{ return &m_pNode;																	}

		protected:
			TStackList& m_list;
			TNode*			m_pNode;
		private:
		}; // End [class TIterator]

		TStackList(void)
			: m_head(m_dummy), m_tail(m_dummy)
		{
			m_head.m_pNext = &m_tail;
			m_tail.m_pPrevious = &m_head;
		}
		virtual ~TStackList(void)	{																														}

		const TNode& Head(void) const { return m_head;	}
		const TNode& Tail(void) const { return m_tail;	}

		void Add(TNode& node)
		{
			TNode* pHere = m_head.m_pNext;
			bool added = false;

			while (!added)
			{
				if (pHere != &m_tail)
				{
					if (node.m_element > pHere->m_element)
					{
						pHere = pHere->m_pNext;
					}
					else
					{
						pHere->Prepend(node);
					}
				}
				else
				{
					m_tail.Prepend(node);
					added = true;
				}
			}
		}

		void Remove(TNode& node)
		{
			TNode* pHere = m_head.m_pNext;

			while (pHere != NULL)
			{
				if (pHere == &node)
				{
					node.Unlink();
					pHere = NULL;
				}
			}
		}

	protected:
		_element m_dummy;
		TNode m_head;
		TNode m_tail;

	private:
	}; // End [class TStackList]
} // End [namespace engine]

//==============================================================================

#endif // End [!defined(__STACK_LIST_H__)]
// [EOF]
