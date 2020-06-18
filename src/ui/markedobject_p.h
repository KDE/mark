#ifndef MARKEDOBJECT_P_H
#define MARKEDOBJECT_P_H

class MarkedClass;

class MarkedObjectPrivate
{
public:
    MarkedObjectPrivate(MarkedClass* markedclass = nullptr) { m_objClass = markedclass; };

public:
    MarkedClass* m_objClass;
};

#endif // MARKEDOBJECT_P_H
