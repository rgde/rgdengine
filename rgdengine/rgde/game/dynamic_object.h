#pragma once

namespace game
{
	// dynamic game object class
	// used to get update notification from game
	class dynamic_object
	{
		friend class game_system;
	public:
		dynamic_object();
		virtual ~dynamic_object();

		virtual void update(float dt) = 0;

	private:
		void unsubscribe() {m_is_subscribed = false;}

	private:
		std::list<dynamic_object*>::iterator m_handle;
		bool m_is_subscribed;
	};
}