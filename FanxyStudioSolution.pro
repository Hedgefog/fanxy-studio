TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS = FanxyStudio \
	  BaseProject \
	  AMXProject

#DEST
CONFIG(release, debug|release): DESTDIR = release
else:CONFIG(debug, debug|release): DESTDIR = debug

#DEPLOYMENT
DEPLOYMENT_PWD = $$PWD/deployment
win32:DEPLOYMENT_PWD ~= s,/,\\,g

EXECUTABLE = FanxyStudio
PROJECT_PLUGINS = BaseProject \
                  AMXProject

win32:EXECUTABLE_EXT = .exe

#EXECUTABLE
EXECUTABLE_PWD = $$PWD/$$EXECUTABLE/$$DESTDIR/$$EXECUTABLE$$EXECUTABLE_EXT
win32:EXECUTABLE_PWD ~= s,/,\\,g
deployCopyComand += $$QMAKE_COPY $$quote($$EXECUTABLE_PWD) $$quote($$DEPLOYMENT_PWD) $$escape_expand(\\n\\t)

#PLUGINS
for(PROJECT, PROJECT_PLUGINS) {
    FILE = $$PWD/$$PROJECT/$$DESTDIR/$$PROJECT"."$$QMAKE_EXTENSION_SHLIB
    DDIR = $$DEPLOYMENT_PWD/plugins/projects

    win32:FILE ~= s,/,\\,g
    win32:DDIR ~= s,/,\\,g

    deployCopyComand += $$QMAKE_COPY $$quote($$FILE) $$quote($$DDIR) $$escape_expand(\\n\\t)
}

!isEmpty(deployCopyComand) {
    copydeployitems.commands = $$deployCopyComand
    first.depends = $(first) copydeployitems
    export(first.depends)
    export(copydeployitems.commands)
    QMAKE_EXTRA_TARGETS += first copydeployitems
}
