#pragma once

#include "AbstractPartFitVisitor.h"

/** Extends the AbstractPartFitVisitor for supporting the packing process. */
class PackingPartFitVisitor : public AbstractPartFitVisitor
{
public:
	PackingPartFitVisitor(PartPtr part, int partId, int count, shared_ptr<DLXSolver> dlxSolver, SetToPartMapPtr locationSetToPart,
		SetToOrientationMapPtr locationSetToOrient);
	
	virtual ~PackingPartFitVisitor();

	void visit(World& box, Point point) override;

private:	
	int _partId;	// The ID of the part, used for the code of the mandatory fields in AlgX matrix
	int _count;	// The number of occurrences of the part in the decomposition solution
};

