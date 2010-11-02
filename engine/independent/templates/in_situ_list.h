#if !defined(__IN_SITU_LIST_H__)
#define __IN_SITU_LIST_H__

//==============================================================================

namespace engine
{
	//============================================================================
	// TInSituList
	//============================================================================
	template <typename _type>
	class TInSituList
	{
	public:
		typedef _type elementType;

		//==========================================================================
		// CElement
		//==========================================================================
		class CElement
		{
		public:
			CElement(TInSituList::elementType& object)
				: m_Next(NULL), m_Previous(NULL), m_containedObject(object)	{}
			virtual ~CElement(void)	{	Unlink();	}

			void InsertAfter(CElement& element)	
			{
				element.Unlink();
				element.m_Previous = this;

				if (m_Next)
				{
					m_Next->m_Previous = &element;
					element.m_Next = m_Next;
				}

				m_Next = &element;
			}

			void InsertBefore(CElement& element)
			{
				element.Unlink();
				element.m_Next = this;

				if (m_Previous)
				{
					m_Previous->m_Next = &element;
					element.m_Previous = m_Previous;
				}

				m_Previous = &element;
			}

			operator TInSituList::elementType&(void)
			{
				return m_containedObject;
			}

			operator >(CElement& rhs)
			{
				return m_containedObject > rhs.m_containedObject;
			}

			operator <(CElement& rhs)
			{
				return m_containedObject < rhs.m_containedObject;
			}

			operator ==(CElement& rhs)
			{
				return m_containedObject == rhs.m_containedObject;
			}

			void Unlink(void)
			{
				if (m_Previous)	m_Previous->m_Next = m_Next;
				if (m_Next)			m_Next->m_Previous = m_Previous;
			}

		protected:
			CElement* m_Next;
			CElement* m_Previous;
			TInSituList::elementType& m_containedObject;

		private:
		}; // End [class CElement]

		//==========================================================================

		class CIterator
		{
		public:
			CIterator(TInSituList& list)
				: m_list(list), m_pElement(list.m_pHead)	{}
			~CIterator(void)														{	m_pElement(NULL);	}

			operator++(CIterator& iterator)
			{
				if (m_pElement == NULL)
				{
					m_pElement = m_list.m_pHead;
				}
				else
				{
					if (m_pElement->m_pNext != NULL)
					{
						m_pElement = m_pElement->m_pNext;
					}
				}
			}

			operator--(CIterator iterator)
			{
				if (m_pElement == NULL)
				{
					m_pElement = m_list.m_pHead;
				}
				else
				{
					if (m_pElement->m_pPrevious != NULL)
					{
						m_pElement = m_pElement->m_pPrevious;
					}
				}
			}

			operator CElement*(void)
			{
				return m_pElement;
			}

			const CElement* Head(void)
			{
				return m_list.m_pHead; 
			}

			const CElement* Tail(void)
			{
				return m_list.m_pTail;
			}

		protected:
			TInSituList& m_list;
			TInSituList::elementType* m_pElement;
		private:
		};

		TInSituList(void)
			: m_pHead(NULL), m_pTail(NULL)	{};
		virtual ~TInSituList(void)				{ m_pHead = m_pTail = NULL;	}

		void Add(CElement& element)
		{
			if (m_pHead)
			{
				CElement* pElement = m_pHead;
				while (pElement != NULL)
				{	
					if (element > *pElement)
					{
						if (pElement->m_Next != NULL)
						{
							pElement = pElement->m_Next;
						}
						else
						{
							m_pTail = &element;
							pElement->InsertAfter(element);
							pElement = NULL;
						}
					}
					else
					{
						if (element == *pElement)
						{
							if ((pElement->m_Next != NULL) && (element == *(pElement->m_Next)))
							{
								pElement = pElement->m_Next;
							}
							else
							{
								if (pElement->m_Next == NULL)
								{
									m_pTail = &element;
								}

								pElement->InsertAfter(element);
								pElement = NULL;
							}
						}
					}
				}
			}
			else
			{
				m_pHead = m_pTail = &element;
			}
		}

		bool Remove(CElement& element)
		{
			CElement* pElement = m_pHead;
			bool removed = false;

			while (pElement != NULL)
			{
				if (element == *pElement)
				{
					if (pElement->m_Next == NULL)
					{
						m_pTail = pElement->m_Previous;
					}

					pElement->Unlink();
					removed = true;
					pElement = NULL;
				}
				else
				{
					pElement = pElement->m_Next;
				}
			}

			return removed;
		}

	protected:
		CElement* m_pHead;
		CElement* m_pTail;

	private:
	}; // End [class TInSituList]
} // End [namespace engine]

//==============================================================================

#endif // End [!defined(__IN_SITU_LIST_H__)]
// [EOF]
