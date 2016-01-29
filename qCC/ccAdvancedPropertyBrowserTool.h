#ifndef PROPERTYINSPECTOR_H
#define PROPERTYINSPECTOR_H

#include <QWidget>

#include <qtpropertybrowser.h>
#include <qttreepropertybrowser.h>
#include <qtvariantproperty.h>
#include <ccDBRoot.h>

class PropertyBrowser: public QtTreePropertyBrowser
{
    Q_OBJECT
public:

    PropertyBrowser(QWidget *parent, ccDBRoot * root);

	void addClassProperties(const QMetaObject *metaObject);
	void updateClassProperties(const QMetaObject *metaObject, bool recursive);
	void saveExpandedState();
	void restoreExpandedState();
	int enumToInt(const QMetaEnum &metaEnum, int enumValue) const;
	int intToEnum(const QMetaEnum &metaEnum, int intValue) const;
	int flagToInt(const QMetaEnum &metaEnum, int flagValue) const;
	int intToFlag(const QMetaEnum &metaEnum, int intValue) const;
	bool isSubValue(int value, int subValue) const;
	bool isPowerOf2(int value) const;

    void setObject(QObject *object, QList<QObject *> mirror = QList<QObject * >());


//    void assignProperties

public slots:
	void slotValueChanged(QtProperty *property, const QVariant &value);

    void selectionChanged();


protected:
    class QtVariantPropertyManager *m_manager;

    class QtVariantPropertyManager *m_readOnlyManager;


    class QObject * m_object = nullptr;

	QMap<const QMetaObject *, QtProperty *> m_classToProperty;
	QMap<QtProperty *, const QMetaObject *> m_propertyToClass;
	QMap<QtProperty *, int>     m_propertyToIndex;

	QMap<const QMetaObject *, QMap<int, QtVariantProperty *> > m_classToIndexToProperty;

	QMap<QtProperty *, bool>    m_propertyToExpanded;
	QList<QtProperty *>         m_topLevelProperties;



    QList<QObject *> m_mirror_objects;


    ccDBRoot * m_dbroot;
};

#endif // PROPERTYINSPECTOR_H
