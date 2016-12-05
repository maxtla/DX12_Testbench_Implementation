#pragma once

struct VertexLayout
{
	/* 
	 * Assume:
	 * 3 floats for POS
	 * 3 floats for NOR
	 * 4 floats for COL
	 * 2 floats for UV
	 */
	enum class LAYOUT { POS, POSNOR, POSNORCOL, POSNORUV };
public:
	/* subclass for API */
	VertexLayout();
	~VertexLayout();
};

