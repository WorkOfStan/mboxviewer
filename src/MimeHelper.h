//
//////////////////////////////////////////////////////////////////
//
//  Windows Mbox Viewer is a free tool to view, search and print mbox mail archives.
//
// Source code and executable can be downloaded from
//  https://sourceforge.net/projects/mbox-viewer/  and
//  https://github.com/eneam/mboxviewer
//
//  Copyright(C) 2019  Enea Mansutti, Zbigniew Minciel
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the version 3 of GNU Affero General Public License
//  as published by the Free Software Foundation; 
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the GNU
//  Library General Public License for more details.
//
//  You should have received a copy of the GNU Library General Public
//  License along with this program; if not, write to the
//  Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
//  Boston, MA  02110 - 1301, USA.
//
//////////////////////////////////////////////////////////////////
//


#pragma once

#include "afxstr.h"
#include "Mime.h"
#include "MimeCode.h"

class CMimeBody;

class MboxCMimeHelper
{
public:
	static void GetContentType(CMimeBody* pBP, CString &value);
	static void GetContentLocation(CMimeBody* pBP, CString &value);
	static void GetTransferEncoding(CMimeBody* pBP, CString &value);
	static void GetContentID(CMimeBody* pBP, CString &value);
	static void GetContentDescription(CMimeBody* pBP, CString &value);
	static void GetContentDisposition(CMimeBody* pBP, CString &value);
	static void GetCharset(CMimeBody* pBP, CString &value);
	static void Name(CMimeBody* pBP, CString &value);
	static void Filename(CMimeBody* pBP, CString &value);
	//
	static bool IsAttachment(CMimeBody* pBP);
	static bool IsInlineAttachment(CMimeBody* pBP);
protected:
	static void GetValue(CMimeBody* pBP, const char* fieldName, CString &value);
};

class MboxCMimeCodeBase64 : public CMimeCodeBase64
{
public:
	MboxCMimeCodeBase64(const char* pbInput, int nInputSize) {
		SetInput(pbInput, nInputSize, false);
	}
};

class MboxCMimeCodeQP : public CMimeCodeQP
{
public:
	MboxCMimeCodeQP(const char* pbInput, int nInputSize) {
		SetInput(pbInput, nInputSize, false);
	}
};

