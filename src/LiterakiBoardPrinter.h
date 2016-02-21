/*
 * LiterakiBoardPrinter.h
 *
 *  Created on: 05-02-2015
 *      Author: saf
 */

#ifndef LITERAKIBOARDPRINTER_H_
#define LITERAKIBOARDPRINTER_H_

#include "BoardPrinter.h"
#include "Fields.h"

class LiterakiBoardPrinter : public BoardPrinter, FieldVisitor {
protected:
	std::wstring fieldString;
	std::wstring tileString;

	virtual std::wstring getTileString(const std::shared_ptr<Tile>& tile,
			const BlankAssignments& blanks) override;
	virtual std::wstring getFieldString(const Field& field,
			const std::shared_ptr<Tile>& tile, const BlankAssignments& blanks) override;
public:
	LiterakiBoardPrinter();
	virtual ~LiterakiBoardPrinter();

	virtual void visit(const PlainField *f);
	virtual void visit(const MultiplicativeLetterBonusField *f);
	virtual void visit(const MultiplicativeWordBonusField *f);
	virtual void visit(const ColoredField *f);
};

#endif /* LITERAKIBOARDPRINTER_H_ */
