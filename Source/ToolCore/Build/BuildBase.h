//
// Copyright (c) 2014-2016 THUNDERBEAST GAMES LLC
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#pragma once

#include <Atomic/Core/Object.h>
#include "BuildTypes.h"
#include "../Platform/Platform.h"

using namespace Atomic;

namespace ToolCore
{

class ResourcePackager;
class Project;

class BuildBase : public Object
{
    OBJECT(BuildBase);

public:

    BuildBase(Context* context, Project* project, PlatformID platform);
    virtual ~BuildBase();

    virtual void Build(const String& buildPath) = 0;

    // some platforms may want to do their own packaging of resources
    virtual bool UseResourcePackager() { return true; }

    virtual String GetBuildSubfolder() = 0;

    // add in search order, first added is first searched
    // will warn on name conflicts
    void AddResourceDir(const String& dir);

    void BuildLog(const String& message, bool sendEvent = true);
    void BuildWarn(const String& warning, bool sendEvent = true);
    void BuildError(const String& error, bool sendEvent = true);

    /// Fail the current build
    void FailBuild(const String& message);

    /// Converts subprocess output event to a buildoutput event
    void HandleSubprocessOutputEvent(StringHash eventType, VariantMap& eventData);

protected:

    bool BuildClean(const String& path);
    bool BuildRemoveDirectory(const String& path);
    bool BuildCreateDirectory(const String& path);
    bool BuildCopyFile(const String& srcFileName, const String& destFileName);
    virtual bool CheckIncludeResourceFile(const String& resourceDir, const String& fileName);

    void GenerateResourcePackage(const String& resourcePackagePath);

    void BuildResourceEntries();

    void GetDefaultResourcePaths(Vector<String>& paths);
    String GetSettingsDirectory();

    String buildPath_;
    PODVector<BuildResourceEntry*> resourceEntries_;

    bool containsMDL_;
    bool buildFailed_;

private:

    PlatformID platformID_;

    Vector<String> buildLog_;
    Vector<String> buildWarnings_;
    Vector<String> buildErrors_;

    void ScanResourceDirectory(const String& resourceDir);

    SharedPtr<Project> project_;
    SharedPtr<ResourcePackager> resourcePackager_;
    Vector<String> resourceDirs_;

};

}
