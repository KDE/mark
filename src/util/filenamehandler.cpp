#include "util/filenamehandler.h"

#include <QMimeDatabase>

const char* FilenameHandler::filterString(Serializer::OutputType outputType)
{
    if (outputType == Serializer::OutputType::XML)
        return "XML files (*.xml *.XML)";
    else if (outputType == Serializer::OutputType::JSON)
        return "JSON files (*.json *.JSON)";
    return "";
}

QString FilenameHandler::placeSuffix(const QString& filename, Serializer::OutputType outputType)
{
    QMimeDatabase mimeDatabase;
    QString outputFilename (filename);
    QString suffix = mimeDatabase.suffixForFileName(filename);
    if (!suffix.isEmpty()) {
        if (suffix == "json" || suffix == "xml")
            outputFilename.replace(".", "_");
        else
            outputFilename.remove("." + suffix);
    }

    outputFilename.append(outputType == Serializer::OutputType::XML ? ".xml" : ".json");
    return outputFilename;
}

bool FilenameHandler::isImageFile(const QString& filepath)
{
    QMimeDatabase database;
    QMimeType mimeType = database.mimeTypeForFile(filepath);
    return mimeType.inherits("image/jpeg") || mimeType.inherits("image/png");
}

bool FilenameHandler::isTextFile(const QString& filepath)
{
    QMimeDatabase database;
    QMimeType mimeType = database.mimeTypeForFile(filepath);
    return mimeType.inherits("text/plain");
}
