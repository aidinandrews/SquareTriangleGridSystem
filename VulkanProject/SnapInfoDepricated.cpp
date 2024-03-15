#include "CoordinateWrappers.h"

#define ORIENTATION_0   0
#define ORIENTATION_30  1
#define ORIENTATION_60  2
#define ORIENTATION_90	 3
#define ORIENTATION_120 4
#define ORIENTATION_150 5
#define ORIENTATION_180 6
#define ORIENTATION_210 7
#define ORIENTATION_240 8
#define ORIENTATION_270 9
#define ORIENTATION_300 10
#define ORIENTATION_330 11

struct ChildAdj {
	XXYY centerAdj;
	int orientation;
	ChildAdj(XXYY center, int orientation)
		: centerAdj(center), orientation(orientation) {

	}
};

// [child shape type][parent shape type][parent orientation][parent section]
const ChildAdj childAdjs[2][2][4][4] = {
	{ // Triangle child added
		{ // to triangle parent
			{ // at parent orientation 0 (0 degrees)
				{ ChildAdj(XXYY(2, 0, 0, -2), ORIENTATION_60) },  // in parent section 0.
				{ ChildAdj(XXYY(0, 0, 0, 4), ORIENTATION_60) },   // in parent section 1.
				{ ChildAdj(XXYY(-2, 0, 0, -2), ORIENTATION_60) }, // in parent section 2.
				{ ChildAdj(XXYY(0, 0, 0, 0), 0) }				  // spacer.
			},
			{ // at parent orientation 1 (30 degrees)
				{ ChildAdj(XXYY(0, 2, -2, 0), ORIENTATION_90) },  // in parent section 0.
				{ ChildAdj(XXYY(0, 2, 2, 0), ORIENTATION_90) },   // in parent section 1.
				{ ChildAdj(XXYY(0, -4, 0, 0), ORIENTATION_90) },  // in parent section 2.
				{ ChildAdj(XXYY(0, 0, 0, 0), 0) }				  // spacer.
			},
			{ // at parent orientation 2 (60 degrees)
				{ ChildAdj(XXYY(0, 0, 0, -4), ORIENTATION_0) },   // in parent section 0.
				{ ChildAdj(XXYY(2, 0, 0, 2), ORIENTATION_0) },    // in parent section 1.
				{ ChildAdj(XXYY(-2, 0, 0, 2), ORIENTATION_0) },   // in parent section 2.
				{ ChildAdj(XXYY(0, 0, 0, 0), 0) }				  // spacer.
			},
			{ // at parent orientation 3 (90 degrees)
				{ ChildAdj(XXYY(0, -2, -2, 0), ORIENTATION_30) }, // in parent section 0.
				{ ChildAdj(XXYY(0, 4, 0, 0), ORIENTATION_30) },   // in parent section 1.
				{ ChildAdj(XXYY(0, -2, 2, 0), ORIENTATION_30) },  // in parent section 2.
				{ ChildAdj(XXYY(0, 0, 0, 0), 0) }				  // spacer.
			}
		},
		{ // to square parent :
			{ // at parent orientation 0 (0 degrees)
				{ ChildAdj(XXYY(0, 0, -2, -2), ORIENTATION_0) },    // in parent section 0.
				{ ChildAdj(XXYY(2, 2, 0, 0), ORIENTATION_30) },     // in parent section 1.
				{ ChildAdj(XXYY(0, 0, 2, 2), ORIENTATION_60) },     // in parent section 2.
				{ ChildAdj(XXYY(-2, -2, 0, 0), ORIENTATION_90) }    // in parent section 3.
			},
			{ // at parent orientation 1 (30 degrees)
				{ ChildAdj(XXYY(-1, -1, -1, -3), ORIENTATION_30) }, // in parent section 0.
				{ ChildAdj(XXYY(1, 3, -1, -1), ORIENTATION_60) },   // in parent section 1.
				{ ChildAdj(XXYY(1, 1, 1, 3), ORIENTATION_90) },     // in parent section 2.
				{ ChildAdj(XXYY(-1, -3, 1, 1), ORIENTATION_0) }		// in parent section 3.
			},
			{ // at parent orientation 2 (60 degrees)
				{ ChildAdj(XXYY(-1, -3, -1, -1), ORIENTATION_60) }, // in parent section 0.
				{ ChildAdj(XXYY(1, 1, -1, -3), ORIENTATION_90) },   // in parent section 1.
				{ ChildAdj(XXYY(1, 3, 1, 1), ORIENTATION_0) },      // in parent section 2.
				{ ChildAdj(XXYY(-1, -1, 1, 3), ORIENTATION_30) }   	// in parent section 3.
			},
			{ // spacer
				{ ChildAdj(XXYY(0, 0, 0, 0), 0) },
				{ ChildAdj(XXYY(0, 0, 0, 0), 0) },
				{ ChildAdj(XXYY(0, 0, 0, 0), 0) },
				{ ChildAdj(XXYY(0, 0, 0, 0), 0) }
			}
		},
	},
	{ // Square child added
		{ // to triangle parent
			{ // at parent orientation 0 (0 degrees)
				{ ChildAdj(XXYY(1, 3, -1, -1), ORIENTATION_30) },   // in parent section 0.
				{ ChildAdj(XXYY(0, 0, 2, 2), ORIENTATION_0) },      // in parent section 1.
				{ ChildAdj(XXYY(-1, -3, -1, -1), ORIENTATION_60) }, // in parent section 2.
				{ ChildAdj(XXYY(0, 0, 0, 0), 0) }				    // spacer.
			},
			{ // at parent orientation 1 (30 degrees)
				{ ChildAdj(XXYY(1, 1, -1, -3), ORIENTATION_60) },   // in parent section 0.
				{ ChildAdj(XXYY(1, 1, 1, 3), ORIENTATION_30) },     // in parent section 1.
				{ ChildAdj(XXYY(-2, -2, 0, 0), ORIENTATION_0) },    // in parent section 2.
				{ ChildAdj(XXYY(0, 0, 0, 0), 0) }				    // spacer.
			},
			{ // at parent orientation 2 (60 degrees)
				{ ChildAdj(XXYY(0, 0, -2, -2), ORIENTATION_0) },    // in parent section 0.
				{ ChildAdj(XXYY(1, 3, 1, 1), ORIENTATION_60) },     // in parent section 1.
				{ ChildAdj(XXYY(-1, -3, 1, 1), ORIENTATION_30) },   // in parent section 2.
				{ ChildAdj(XXYY(0, 0, 0, 0), 0) }				    // spacer.
			},
			{ // at parent orientation 3 (90 degrees)
				{ ChildAdj(XXYY(-1, -1, -1, -3), ORIENTATION_30) }, // in parent section 0.
				{ ChildAdj(XXYY(2, 2, 0, 0), ORIENTATION_0) },      // in parent section 1.
				{ ChildAdj(XXYY(-1, -1, 1, 3), ORIENTATION_60) },   // in parent section 2.
				{ ChildAdj(XXYY(0, 0, 0, 0), 0) }				    // spacer.
			}
		},
		{ // to square parent
			{ // at parent orientation 0 (0 degrees)
				{ ChildAdj(XXYY(0, 0, -4, 0), ORIENTATION_0) },   // in parent section 0.
				{ ChildAdj(XXYY(4, 0, 0, 0), ORIENTATION_0) },    // in parent section 1.
				{ ChildAdj(XXYY(0, 0, 4, 0), ORIENTATION_0) },    // in parent section 2.
				{ ChildAdj(XXYY(-4, 0, 0, 0), ORIENTATION_0) }    // in parent section 3.
			},
			{ // at parent orientation 1 (30 degrees)
				{ ChildAdj(XXYY(-2, 0, 0, -6), ORIENTATION_30) }, // in parent section 0.
				{ ChildAdj(XXYY(0, 6, -2, 0), ORIENTATION_30) },  // in parent section 1.
				{ ChildAdj(XXYY(2, 0, 0, 6), ORIENTATION_30) },   // in parent section 2.
				{ ChildAdj(XXYY(0, -6, 2, 0), ORIENTATION_30) }	  // in parent section 3.
			},
			{ // at parent orientation 2 (60 degrees)
				{ ChildAdj(XXYY(0, -6, -2, 0), ORIENTATION_60) }, // in parent section 0.
				{ ChildAdj(XXYY(2, 0, 0, -6), ORIENTATION_60) },  // in parent section 1.
				{ ChildAdj(XXYY(0, 6, 2, 0), ORIENTATION_60) },   // in parent section 2.
				{ ChildAdj(XXYY(-2, 0, 0, 6), ORIENTATION_60) }   // in parent section 3.
			},
			{ // spacer
				{ ChildAdj(XXYY(0, 0, 0, 0), 0) },
				{ ChildAdj(XXYY(0, 0, 0, 0), 0) },
				{ ChildAdj(XXYY(0, 0, 0, 0), 0) },
				{ ChildAdj(XXYY(0, 0, 0, 0), 0) }
			}
		},
	}
};

// [child shape type][parent shape type][parent orientation][parent section][child orientation]
const bool canSnap[2][2][4][4][4] = {
	{ // Triangle child added
		{ // to triangle parent
			{ // at parent orientation 0 (0 degrees)
				{ // in parent section 0,
					{ false }, // with child orientation 0 (0 degrees).
					{ false }, // with child orientation 1 (30 degrees).
					{ true  }, // with child orientation 2 (60 degrees).
					{ false }, // with child orientation 3 (90 degrees).
				},
				{ // in parent section 1,
					{ false }, // with child orientation 0 (0 degrees).
					{ false }, // with child orientation 1 (30 degrees).
					{ true  }, // with child orientation 2 (60 degrees).
					{ false }, // with child orientation 3 (90 degrees).
				},
				{ // in parent section 2,
					{ false }, // with child orientation 0 (0 degrees).
					{ false }, // with child orientation 1 (30 degrees).
					{ true  }, // with child orientation 2 (60 degrees).
					{ false }, // with child orientation 3 (90 degrees).
				},
				{{0},{0},{0},{0}} // spacer.
			},
			{ // at parent orientation 1 (30 degrees)
				{ // in parent section 0,
					{ false }, // with child orientation 0 (0 degrees).
					{ false }, // with child orientation 1 (30 degrees).
					{ false }, // with child orientation 2 (60 degrees).
					{ true  }, // with child orientation 3 (90 degrees).
				},
				{ // in parent section 1,
					{ false }, // with child orientation 0 (0 degrees).
					{ false }, // with child orientation 1 (30 degrees).
					{ false }, // with child orientation 2 (60 degrees).
					{ true  }, // with child orientation 3 (90 degrees).
				},
				{ // in parent section 2,
					{ false }, // with child orientation 0 (0 degrees).
					{ false }, // with child orientation 1 (30 degrees).
					{ false }, // with child orientation 2 (60 degrees).
					{ true  }, // with child orientation 3 (90 degrees).
				},
				{{0},{0},{0},{0}} // spacer.
			},
			{ // at parent orientation 2 (60 degrees)
				{ // in parent section 0,
					{ true  }, // with child orientation 0 (0 degrees).
					{ false }, // with child orientation 1 (30 degrees).
					{ false }, // with child orientation 2 (60 degrees).
					{ false }, // with child orientation 3 (90 degrees).
				},
				{ // in parent section 1,
					{ true  }, // with child orientation 0 (0 degrees).
					{ false }, // with child orientation 1 (30 degrees).
					{ false }, // with child orientation 2 (60 degrees).
					{ false }, // with child orientation 3 (90 degrees).
				},
				{ // in parent section 2,
					{ true  }, // with child orientation 0 (0 degrees).
					{ false }, // with child orientation 1 (30 degrees).
					{ false }, // with child orientation 2 (60 degrees).
					{ false }, // with child orientation 3 (90 degrees).
				},
				{{0},{0},{0},{0}} // spacer.
			},
			{ // at parent orientation 3 (90 degrees)
				{ // in parent section 0,
					{ false }, // with child orientation 0 (0 degrees).
					{ true  }, // with child orientation 1 (30 degrees).
					{ false }, // with child orientation 2 (60 degrees).
					{ false }, // with child orientation 3 (90 degrees).
				},
				{ // in parent section 1,
					{ false }, // with child orientation 0 (0 degrees).
					{ true  }, // with child orientation 1 (30 degrees).
					{ false }, // with child orientation 2 (60 degrees).
					{ false }, // with child orientation 3 (90 degrees).
				},
				{ // in parent section 2,
					{ false }, // with child orientation 0 (0 degrees).
					{ true  }, // with child orientation 1 (30 degrees).
					{ false }, // with child orientation 2 (60 degrees).
					{ false }, // with child orientation 3 (90 degrees).
				},
				{{0},{0},{0},{0}} // spacer.
			}
		},
		{ // to square parent,
			{ // at parent orientation 0 (0 degrees),
				{ // in parent section 0,
					{ true  }, // with child orientation 0 (0 degrees).
					{ false }, // with child orientation 1 (30 degrees).
					{ false }, // with child orientation 2 (60 degrees).
					{ false }, // with child orientation 3 (90 degrees).
				},
				{ // in parent section 1,
					{ false }, // with child orientation 0 (0 degrees).
					{ true  }, // with child orientation 1 (30 degrees).
					{ false }, // with child orientation 2 (60 degrees).
					{ false }, // with child orientation 3 (90 degrees).
				},
				{ // in parent section 2,
					{ false }, // with child orientation 0 (0 degrees).
					{ false }, // with child orientation 1 (30 degrees).
					{ true  }, // with child orientation 2 (60 degrees).
					{ false }, // with child orientation 3 (90 degrees).
				},
				{ // in parent section 3,
					{ false }, // with child orientation 0 (0 degrees).
					{ false }, // with child orientation 1 (30 degrees).
					{ false }, // with child orientation 2 (60 degrees).
					{ true  }, // with child orientation 3 (90 degrees).
				}
			},
			{ // at parent orientation 1 (30 degrees)
				{ // in parent section 0,
					{ false }, // with child orientation 0 (0 degrees).
					{ true  }, // with child orientation 1 (30 degrees).
					{ false }, // with child orientation 2 (60 degrees).
					{ false }, // with child orientation 3 (90 degrees).
				},
				{ // in parent section 1,
					{ false }, // with child orientation 0 (0 degrees).
					{ false }, // with child orientation 1 (30 degrees).
					{ true  }, // with child orientation 2 (60 degrees).
					{ false }, // with child orientation 3 (90 degrees).
				},
				{ // in parent section 2,
					{ false }, // with child orientation 0 (0 degrees).
					{ false }, // with child orientation 1 (30 degrees).
					{ false }, // with child orientation 2 (60 degrees).
					{ true  }, // with child orientation 3 (90 degrees).
				},
				{ // in parent section 3,
					{ true  }, // with child orientation 0 (0 degrees).
					{ false }, // with child orientation 1 (30 degrees).
					{ false }, // with child orientation 2 (60 degrees).
					{ false }, // with child orientation 3 (90 degrees).
				}
			},
			{ // at parent orientation 2 (60 degrees)
				{ // in parent section 0,
					{ false }, // with child orientation 0 (0 degrees).
					{ false }, // with child orientation 1 (30 degrees).
					{ true  }, // with child orientation 2 (60 degrees).
					{ false }, // with child orientation 3 (90 degrees).
				},
				{ // in parent section 1,
					{ false }, // with child orientation 0 (0 degrees).
					{ false }, // with child orientation 1 (30 degrees).
					{ false }, // with child orientation 2 (60 degrees).
					{ true  }, // with child orientation 3 (90 degrees).
				},
				{ // in parent section 2,
					{ true  }, // with child orientation 0 (0 degrees).
					{ false }, // with child orientation 1 (30 degrees).
					{ false }, // with child orientation 2 (60 degrees).
					{ false }, // with child orientation 3 (90 degrees).
				},
				{ // in parent section 3,
					{ false }, // with child orientation 0 (0 degrees).
					{ true  }, // with child orientation 1 (30 degrees).
					{ false }, // with child orientation 2 (60 degrees).
					{ false }, // with child orientation 3 (90 degrees).
				}
			},
			{{{0},{0},{0},{0}},{{0},{0},{0},{0}},{{0},{0},{0},{0}},{{0},{0},{0},{0}}} // spacer.
		},
	},
	{ // Square child added
		{ // to triangle parent
			{ // at parent orientation 0 (0 degrees)
				{ // in parent section 0,
					{ false }, // with child orientation 0 (0 degrees).
					{ true  }, // with child orientation 1 (30 degrees).
					{ false }, // with child orientation 2 (60 degrees).
					{0}, // spacer.
				},
				{ // in parent section 1,
					{ true  }, // with child orientation 0 (0 degrees).
					{ false }, // with child orientation 1 (30 degrees).
					{ false }, // with child orientation 2 (60 degrees).
					{0}, // spacer.
				},
				{ // in parent section 2,
					{ false }, // with child orientation 0 (0 degrees).
					{ false }, // with child orientation 1 (30 degrees).
					{ true  }, // with child orientation 2 (60 degrees).
					{0}, // spacer.
				},
				{{0},{0},{0},{0}} // spacer.
			},
			{ // at parent orientation 1 (30 degrees)
				{ // in parent section 0,
					{ false }, // with child orientation 0 (0 degrees).
					{ false }, // with child orientation 1 (30 degrees).
					{ true  }, // with child orientation 2 (60 degrees).
					{0}, // spacer.
				},
				{ // in parent section 1,
					{ false }, // with child orientation 0 (0 degrees).
					{ true  }, // with child orientation 1 (30 degrees).
					{ false }, // with child orientation 2 (60 degrees).
					{0}, // spacer.
				},
				{ // in parent section 2,
					{ true  }, // with child orientation 0 (0 degrees).
					{ false }, // with child orientation 1 (30 degrees).
					{ false }, // with child orientation 2 (60 degrees).
					{0}, // spacer.
				},
				{{0},{0},{0},{0}} // spacer.
			},
			{ // at parent orientation 2 (60 degrees)
				{ // in parent section 0,
					{ true  }, // with child orientation 0 (0 degrees).
					{ false }, // with child orientation 1 (30 degrees).
					{ false }, // with child orientation 2 (60 degrees).
					{0}, // spacer.
				},
				{ // in parent section 1,
					{ false }, // with child orientation 0 (0 degrees).
					{ false }, // with child orientation 1 (30 degrees).
					{ true  }, // with child orientation 2 (60 degrees).
					{0}, // spacer.
				},
				{ // in parent section 2,
					{ false }, // with child orientation 0 (0 degrees).
					{ true  }, // with child orientation 1 (30 degrees).
					{ false }, // with child orientation 2 (60 degrees).
					{0}, // spacer.
				},
				{{0},{0},{0},{0}} // spacer.
			},
			{ // at parent orientation 3 (90 degrees)
				{ // in parent section 0,
					{ false }, // with child orientation 0 (0 degrees).
					{ true  }, // with child orientation 1 (30 degrees).
					{ false }, // with child orientation 2 (60 degrees).
					{0}, // spacer.
				},
				{ // in parent section 1,
					{ true  }, // with child orientation 0 (0 degrees).
					{ false }, // with child orientation 1 (30 degrees).
					{ false }, // with child orientation 2 (60 degrees).
					{0}, // spacer.
				},
				{ // in parent section 2,
					{ false }, // with child orientation 0 (0 degrees).
					{ false }, // with child orientation 1 (30 degrees).
					{ true  }, // with child orientation 2 (60 degrees).
					{0}, // spacer.
				},
				{ // in parent section 3,
					{ false }, // with child orientation 0 (0 degrees).
					{ false }, // with child orientation 1 (30 degrees).
					{ true  }, // with child orientation 2 (60 degrees).
					{0}, // spacer.
				},
			}
		},
		{ // to square parent
			{ // at parent orientation 0 (0 degrees)
				{ // in parent section 0,
					{ true  }, // with child orientation 0 (0 degrees).
					{ false }, // with child orientation 1 (30 degrees).
					{ false }, // with child orientation 2 (60 degrees).
					{0}, // spacer.
				},
				{ // in parent section 1,
					{ true  }, // with child orientation 0 (0 degrees).
					{ false }, // with child orientation 1 (30 degrees).
					{ false }, // with child orientation 2 (60 degrees).
					{0}, // spacer.
				},
				{ // in parent section 2,
					{ true  }, // with child orientation 0 (0 degrees).
					{ false }, // with child orientation 1 (30 degrees).
					{ false }, // with child orientation 2 (60 degrees).
					{0}, // spacer.
				},
				{ // in parent section 3,
					{ true  }, // with child orientation 0 (0 degrees).
					{ false }, // with child orientation 1 (30 degrees).
					{ false }, // with child orientation 2 (60 degrees).
					{0}, // spacer.
				},
			},
			{ // at parent orientation 1 (30 degrees)
				{ // in parent section 0,
					{ false }, // with child orientation 0 (0 degrees).
					{ true  }, // with child orientation 1 (30 degrees).
					{ false }, // with child orientation 2 (60 degrees).
					{0}, // spacer.
				},
				{ // in parent section 1,
					{ false }, // with child orientation 0 (0 degrees).
					{ true  }, // with child orientation 1 (30 degrees).
					{ false }, // with child orientation 2 (60 degrees).
					{0}, // spacer.
				},
				{ // in parent section 2,
					{ false }, // with child orientation 0 (0 degrees).
					{ true  }, // with child orientation 1 (30 degrees).
					{ false }, // with child orientation 2 (60 degrees).
					{0}, // spacer.
				},
				{ // in parent section 3,
					{ false }, // with child orientation 0 (0 degrees).
					{ true  }, // with child orientation 1 (30 degrees).
					{ false }, // with child orientation 2 (60 degrees).
					{0}, // spacer.
				},
			},
			{ // at parent orientation 2 (60 degrees)
				{ // in parent section 0,
					{ false }, // with child orientation 0 (0 degrees).
					{ false }, // with child orientation 1 (30 degrees).
					{ true  }, // with child orientation 2 (60 degrees).
					{0}, // spacer.
				},
				{ // in parent section 1,
					{ false }, // with child orientation 0 (0 degrees).
					{ false }, // with child orientation 1 (30 degrees).
					{ true  }, // with child orientation 2 (60 degrees).
					{0}, // spacer.
				},
				{ // in parent section 2,
					{ false }, // with child orientation 0 (0 degrees).
					{ false }, // with child orientation 1 (30 degrees).
					{ true  }, // with child orientation 2 (60 degrees).
					{0}, // spacer.
				},
				{ // in parent section 3,
					{ false }, // with child orientation 0 (0 degrees).
					{ false }, // with child orientation 1 (30 degrees).
					{ true  }, // with child orientation 2 (60 degrees).
					{0}, // spacer.
				},
			},
			{{{0},{0},{0},{0}},{{0},{0},{0},{0}},{{0},{0},{0},{0}},{{0},{0},{0},{0}}} // spacer.
		},
	}
};