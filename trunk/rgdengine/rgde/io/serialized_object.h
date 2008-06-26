#pragma once

namespace io
{
	class ISerializedObject
	{
		friend class IWriteStream;
		friend class IReadStream;

	protected:
		virtual void toStream(IWriteStream& wf) const = 0;
		virtual void fromStream(IReadStream& rf) = 0;

	public:
		bool write(IWriteStream& wf) const
		{
			toStream(wf);
			// TODO: catch any exception, if so - return false;
			return true;
		}

		bool read(IReadStream& rf)
		{
			fromStream(rf);
			// TODO: catch any exception, if so - return false;
			return true;
		}
	};
}