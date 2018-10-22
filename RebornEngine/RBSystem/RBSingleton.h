#ifndef RBSINGLETON_H
#define RBSINGLETON_H



namespace RebornEngine
{
	template <typename T>
	class RBSingleton
	{
	protected:
		RBSingleton() {}

	public:
		virtual ~RBSingleton() {}

		static T& Instance()
		{
			static T instance;
			return instance;
		}
	};
}

#endif
