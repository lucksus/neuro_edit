#ifndef SYNTAX_HIGHLIGHTER_H
#define SYNTAX_HIGHLIGHTER_H

#include <QSyntaxHighlighter>

class QTextDocument;
class ScriptSyntaxHighlighter : public QSyntaxHighlighter
{
	Q_OBJECT
public:
        ScriptSyntaxHighlighter(QTextEdit *parent);

protected:
	void highlightBlock(const QString &text);

private:
	struct HighlightingRule
	{
		QRegExp pattern;
		QTextCharFormat format;
	};
	QVector<HighlightingRule> highlightingRules;

	QRegExp commentStartExpression;
	QRegExp commentEndExpression;

	QTextCharFormat keywordFormat;
	QTextCharFormat classFormat;
	QTextCharFormat singleLineCommentFormat;
	QTextCharFormat multiLineCommentFormat;
	QTextCharFormat quotationFormat;
	QTextCharFormat functionFormat;
        QTextCharFormat neuroeditKeywordFormat;
        QTextCharFormat neuroeditFunctionFormat;
};
#endif // SYNTAX_HIGHLIGHTER_H
