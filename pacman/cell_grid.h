#pragma once

/****************************************************************************
	��������������� ������ ��� ����������� ����� �������� �����������
	������� ��������
*****************************************************************************/

namespace Pegas
{
	template<class T>
	class CellGrid;

	/*
		������. ������������� ������ � ��������, ���������� ������ ������,
		� ����� � �������� �������.
	*/

	template<class T>
	class Cell
	{
	public:
		typedef typename std::set<T> ObjectList;
		typedef typename ObjectList::iterator ObjectListIt;

		//�������� ��������� �� �������� ������, � �������� ��������� �����������
		//����� �� 0 �� 7 - ������ ������. ������ ��������� � ������� �� ������� �������,
		//������� ���, ��� ���������� �� ���� ������ ���� ������ � � ��� �� �������
		//���� �������� ������ ���������� (�������� ��� ����� �� �������� �����) - ������ 0,
		//��������� ������� ��������� �� ����. 
		Cell<T>* getSibling(int32 index);
		/*
			�������� �������� ����������� �� ������ ������ � ������
		*/
		ObjectListIt begin();
		ObjectListIt end();
		
	private:
		friend class CellGrid<T>; 
		//��� ������� ���������� ������� CellGrid

		//������������� ������ � ������� ������ � �������
		void init(CellGrid<T>* owner, int32 row, int32 column);
		//��������� ������ � ������
		void addObject(const T& object);
		//������� ������ �� ������
		void removeObject(const T& object);	

	private:
		CellGrid<T>*	m_owner;
		ObjectList		m_objects;
		int32			m_row;
		int32			m_column;
	};

	/*
		����������, �����
	*/
	template<class T>
	class CellGrid
	{
	public:
		CellGrid(): m_cells(0) {}

		/*
			�������� �����. � �������� ���������� ����������� ������ � ������ 
			(�� ������� �������� ������) � ������ ������ (�� ������� ������ �������� �������
			� ����)
		*/
		//void create(float gridWidth, float gridHeight, float cellLength);
		void create(int32 numRows, int32 numColumns);
		void destroy();

		void setArea(float left, float top, float width, float height);

		/*
			��������� ������� ������ � ������. ���� ������ ��� ���������� � �����, �� �������������
			� ������ ������. ������� ������� ������� ��� ������� ������ ���, ����� �� ������� ���������
		*/
		void placeToGrid(const Vector3& position, const T& obj);
		/*
		  ������� ������ �� ������. ������� ������� ������� ��� ������� � ����� ��� ����������� �������
		*/
		void removeObject(const T& obj);
		/*
		  �������� ������ ��� ������ �����
		*/
		Cell<T>* getCell(const Vector3& position);
		Cell<T>* getCell(int32 row, int32 column);

	private:
		Cell<T>** m_cells;
		int32    m_numRows;
		int32    m_numColumns;

		float		m_left;
		float		m_top;	
		float		m_width;
		float		m_height;
		float		m_cellWidth;
		float		m_cellHeight; 

		typedef typename std::map<T, Cell<T>*>  CellsLookupMap;
		CellsLookupMap m_lookupMap;

	private:
		CellGrid(const CellGrid& src);
		CellGrid& operator=(const CellGrid& src);
	};

	/*******************************************************************************************
		Cell class functions
	*******************************************************************************************/
	template<class T>
	inline Cell<T>* Cell<T>::getSibling(int32 index)
	{
		switch(index)
		{
		case 0:
			return m_owner->getCell(m_row + 1, m_column);
		case 1:
			return m_owner->getCell(m_row + 1, m_column + 1);
		case 2:
			return m_owner->getCell(m_row, m_column + 1);
		case 3:
			return m_owner->getCell(m_row - 1, m_column + 1);
		case 4:
			return m_owner->getCell(m_row - 1, m_column);
		case 5:
			return m_owner->getCell(m_row - 1, m_column - 1);
		case 6:
			return m_owner->getCell(m_row, m_column - 1);
		case 7:
			return m_owner->getCell(m_row + 1, m_column - 1);
		default:
			return this;		
		}

		return 0;
	};

	template<class T>
	inline typename Cell<T>::ObjectListIt Cell<T>::begin()
	{
		return m_objects.begin(); 
	};

	template<class T>
	inline typename Cell<T>::ObjectListIt Cell<T>::end()
	{
		return m_objects.end();
	};

	template<class T>
	inline void Cell<T>::init(CellGrid<T>* owner, int32 row, int32 column)
	{
		m_owner = owner;
		m_row = row;
		m_column = column;
	};

	template<class T>
	inline void Cell<T>::addObject(const T& object)
	{
		if(m_objects.count(object) == 0)
			m_objects.insert(object);
	};

	template<class T>
	inline void Cell<T>::removeObject(const T& object)
	{
		if(m_objects.count(object) > 0)
			m_objects.erase(object);
	};

	/***************************************************************************
		CellGrid class implementation
	*****************************************************************************/
	/*
	template<class T>
	inline void CellGrid<T>::create(float gridWidth, float gridHeight, float cellLength)
	{
		assert(gridWidth > 0 && "invalid argument");
		assert(gridHeight > 0 && "invalid argument");
		assert(cellLength > 0 && "invalid argument");

		m_width = gridWidth;
		m_height = gridHeight;
		m_cellLength = cellLength;
		m_numRows = ceil(gridWidth / cellLength);
		m_numColumns = ceil(gridHeight / cellLength);

		m_cells = new Cell<T>*[m_numRows];
		for(size_t row = 0; row < m_numRows; row++)
		{
			m_cells[row] = new Cell<T>[m_numColumns];
			for(size_t column = 0; column < m_numColumns; column++)
			{
				m_cells[row][column].init(this, row, column);
			}
		}
	};*/

	template<class T>
	inline void CellGrid<T>::create(int32 numRows, int32 numColumns)
	{
		assert(numRows > 0);
		assert(numColumns > 0);

		m_numRows = numRows;
		m_numColumns = numColumns;
		
		m_cells = new Cell<T>*[m_numRows];
		for(size_t row = 0; row < m_numRows; row++)
		{
			m_cells[row] = new Cell<T>[m_numColumns];
			for(size_t column = 0; column < m_numColumns; column++)
			{
				m_cells[row][column].init(this, row, column);
			}
		}
	}

	template<class T>
	inline void CellGrid<T>::destroy()
	{
		if(m_cells)
		{
			for(size_t row = 0; row < m_numRows; row++)
			{
				delete[] m_cells[row];		
			}

			delete[] m_cells;
			m_cells = 0;
		}

		m_lookupMap.clear();
	};

	template<class T>
	void CellGrid<T>::setArea(float left, float top, float width, float height)
	{
		assert(m_numRows > 0);
		assert(m_numColumns > 0);

		m_left =  left;
		m_top = top;	
		m_width = width;
		m_height =  height;
		m_cellWidth = m_width / m_numColumns;
		m_cellHeight = m_height / m_numRows;	
	}

	template<class T>
	inline void CellGrid<T>::placeToGrid(const Vector3& position, const T& obj)
	{
		removeObject(obj);

		Cell<T>* cell = getCell(position);
		if(cell)
		{
			cell->addObject(obj);
			m_lookupMap[obj] = cell;
		}
	};

	template<class T>
	inline void CellGrid<T>::removeObject(const T& obj)
	{
		if(m_lookupMap.count(obj) > 0)
		{
			m_lookupMap[obj]->removeObject(obj);
			m_lookupMap.erase(obj);
		}
	};

	template<class T>
	inline Cell<T>* CellGrid<T>::getCell(const Vector3& position)
	{
		size_t row = floor(((position._x - m_left) / m_width) * m_numRows);
		size_t column = floor(((position._y - m_top) / m_height) * m_numColumns);

		return getCell(row, column);
	};

	template<class T>
	inline Cell<T>* CellGrid<T>::getCell(int32 row, int32 column)
	{
		if(row >= 0 && row < m_numRows && column >= 0 && column <  m_numColumns)
		{
			return &m_cells[row][column];
		}
		return 0;
	};
}