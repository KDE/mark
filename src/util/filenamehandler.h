#ifndef FILENAMEHANDLER_H
#define FILENAMEHANDLER_H

#include "core/serializer.h"

#include <QString>

class FilenameHandler
{
public:

    /** @return a filterString containing the supported file extension to given outputType. */
    static const char* filterString(Serializer::OutputType outputType);

    /** @return a string with the suffix based on the outputType. */
    static QString placeSuffix(const QString& filepath, Serializer::OutputType outputType);

    /** Verify if the file is a jpg or png file. */
    static bool isImageFile(const QString& filepath);

    /** Verify if the file is a text file based on its filepath and contents. */
    static bool isTextFile(const QString& filepath);
};

#endif
