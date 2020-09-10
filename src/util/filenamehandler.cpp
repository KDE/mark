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

QString FilenameHandler::placeSuffix(const QString& filepath, Serializer::OutputType outputType)
{
    QMimeDatabase mimeDatabase;
    QString outputFilepath (filepath);
    QString suffix = mimeDatabase.suffixForFileName(filepath);
    if (!suffix.isEmpty()) {
        if (suffix == "json" || suffix == "xml")
            outputFilepath.replace(".", "_");
        else
            outputFilepath.remove("." + suffix);
    }

    outputFilepath.append(outputType == Serializer::OutputType::XML ? ".xml" : ".json");
    return outputFilepath;
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
