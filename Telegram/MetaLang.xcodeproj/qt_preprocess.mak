#############################################################################
# Makefile for building: ../Mac/DebugLang/MetaLang.app/Contents/MacOS/MetaLang
# Generated by qmake (3.0) (Qt 5.5.1)
# Project:  MetaLang.pro
# Template: app
# Command: /usr/local/Qt-5.5.1/bin/qmake -spec macx-xcode -o MetaLang.xcodeproj/project.pbxproj MetaLang.pro
#############################################################################

MAKEFILE      = project.pbxproj

MOC       = /usr/local/Qt-5.5.1/bin/moc
UIC       =
LEX       = flex
LEXFLAGS  =
YACC      = yacc
YACCFLAGS = -d
DEFINES       = -D_DEBUG -DQT_GUI_LIB -DQT_CORE_LIB
INCPATH       = -I/usr/local/Qt-5.5.1/mkspecs/macx-clang -I. -I../../Libraries/QtStatic/qtbase/include/QtGui/5.5.1/QtGui -I../../Libraries/QtStatic/qtbase/include/QtCore/5.5.1/QtCore -I../../Libraries/QtStatic/qtbase/include -I/usr/local/Qt-5.5.1/include -I/usr/local/Qt-5.5.1/include/QtGui -I/usr/local/Qt-5.5.1/include/QtCore -IGeneratedFiles/Debug -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.9.sdk/System/Library/Frameworks/OpenGL.framework/Versions/A/Headers -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.9.sdk/System/Library/Frameworks/AGL.framework/Headers
DEL_FILE  = rm -f
MOVE      = mv -f

IMAGES =
PARSERS =
preprocess: $(PARSERS) compilers
clean preprocess_clean: parser_clean compiler_clean

parser_clean:
mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

check: first

compilers: GeneratedFiles/Debug/moc_genlang.cpp
compiler_objective_c_make_all:
compiler_objective_c_clean:
compiler_rcc_make_all:
compiler_rcc_clean:
compiler_moc_header_make_all: GeneratedFiles/Debug/moc_genlang.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) GeneratedFiles/Debug/moc_genlang.cpp
GeneratedFiles/Debug/moc_genlang.cpp: ../../Libraries/QtStatic/qtbase/include/QtCore/QMap \
		../../Libraries/QtStatic/qtbase/include/QtCore/QVector \
		../../Libraries/QtStatic/qtbase/include/QtCore/QFile \
		../../Libraries/QtStatic/qtbase/include/QtCore/QDir \
		../../Libraries/QtStatic/qtbase/include/QtCore/QRegularExpression \
		../../Libraries/QtStatic/qtbase/include/QtGui/QImage \
		../../Libraries/QtStatic/qtbase/include/QtGui/QPixmap \
		../../Libraries/QtStatic/qtbase/include/QtGui/QPainter \
		../../Libraries/QtStatic/qtbase/include/QtCore/QTextStream \
		../../Libraries/QtStatic/qtbase/include/QtCore/QString \
		../../Libraries/QtStatic/qtbase/include/QtCore/QCoreApplication \
		../../Libraries/QtStatic/qtbase/include/QtGui/QGuiApplication \
		SourceFiles/_other/genlang.h
	/usr/local/Qt-5.5.1/bin/moc $(DEFINES) -D__APPLE__ -D__GNUC__=4 -I/usr/local/Qt-5.5.1/mkspecs/macx-clang -I. -I/usr/local/Qt-5.5.1/include/QtGui/5.5.1/QtGui -I/usr/local/Qt-5.5.1/include/QtCore/5.5.1/QtCore -I/usr/local/Qt-5.5.1/include -I/usr/local/Qt-5.5.1/include/QtGui -I/usr/local/Qt-5.5.1/include/QtCore -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.9.sdk/usr/include/c++/4.2.1 -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.9.sdk/usr/include/c++/4.2.1/backward -I/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang/5.1/include -I/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.9.sdk/usr/include SourceFiles/_other/genlang.h -o GeneratedFiles/Debug/moc_genlang.cpp

compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_rez_source_make_all:
compiler_rez_source_clean:
compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean
