#ifndef MARKEDOBJECT_H
#define MARKEDOBJECT_H

class MarkedObject
{
public:
    explicit MarkedObject(MarkedClass* objClass);

    MarkedClass* objClass() const { return m_objClass; }
    void setObjClass(MarkedClass* objClass) { m_objClass = objClass; }

    void append(const QVariant& item) { m_items << item; }
    const QVariant item(int index) const { return m_items[index]; }
    void setItem(int index, const QVariant& item) { m_items[index] = item; }
    const int size() const { return m_items.size(); }
    void pop_back() { m_items.pop_back(); }

private:
    MarkedClass* m_objClass;
    QVector<QVariant> m_items;
};

#endif // MARKEDOBJECT_H
