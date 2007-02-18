//======== Copyright © 2004 - 2005, viv software, All rights reserved =========
//
// File: message.h
// Author: Vashchaev 'viv' Ivan
// Purpose: $$$
//
//=============================================================================

#ifndef FORMS_MESSAGE_H
#define FORMS_MESSAGE_H

namespace Forms
{
	// Windows message wrapper class
	class Message
	{
	public:
		const UINT	uMsg;
		WPARAM		wParam;
		LPARAM		lParam;
		LRESULT		lResult;

		Message(UINT uMsg, WPARAM wParam, LPARAM lParam)
			: uMsg(uMsg),
			  wParam(wParam),
			  lParam(lParam),
			  lResult(NULL)
		{
		}

		Message(const Message &msg)
			: uMsg(msg.uMsg),
			  wParam(msg.wParam),
			  lParam(msg.lParam),
			  lResult(NULL)
		{
		}
	};

	template <UINT uMsg>
	class MessageT : public Message
	{
	public:
		MessageT(const Message &msg)
			: Message(msg)
		{
		}
	};

	// WM_COMMAND
	template <>
	class MessageT<WM_COMMAND> : public Message
	{
	public:
		MessageT(const Message &msg)
			: Message(msg)
		{
		}

		HWND Handle() const
		{
			return (HWND)lParam;
		}

		int ID() const
		{
			return LOWORD(wParam);
		}

		int Code() const
		{
			return HIWORD(wParam);
		}
	};

	// WM_NOTIFY
	template <>
	class MessageT<WM_NOTIFY> : public Message
	{
	public:
		MessageT(const Message &msg)
			: Message(msg)
		{
		}

		NMHDR * HDR() const
		{
			return (LPNMHDR)lParam;
		}

		HWND Handle() const
		{
			return HDR()->hwndFrom;
		}

		int Code() const
		{
			return HDR()->code;
		}
	};
}

#endif