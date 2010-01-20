#pragma once

namespace gui
{
class BaseWindow;
namespace events
{
struct BaseEventType {};

struct ResizeEvent : public BaseEventType {};
struct ClickEvent : public BaseEventType {};
struct TrackEvent : public BaseEventType {};
struct SizedEvent : public BaseEventType {};
struct MovedEvent : public BaseEventType {};

struct NamedEvent : public BaseEventType 
{
	NamedEvent(const std::string name, BaseWindow* sender) : m_name(name), m_sender(sender) {}
	std::string m_name;
	BaseWindow* m_sender;
};

}
}
