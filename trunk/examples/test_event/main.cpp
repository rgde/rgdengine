//main.cpp
//#include <event/event.h>
//#include <string>
//#include <iostream>

////-----------------------------------------------
//typedef event::TEvent<std::wstring> CSayEvent;
//
////-----------------------------------------------
//struct Named
//{
//    Named(std::wstring n): name(n) {};
//    std::wstring name;
//};
//
////-----------------------------------------------
////шпион
//class CSpy: public event::CListener, public Named
//{
//public:
//    CSpy(std::wstring n): Named(n) {}
//
//    void spy(CSayEvent event)
//    {
//        std::wcout << name << L" подслушал: " << event.m_data << std::endl;
//    }
//};
//
////мужчина
//class CMan: public event::CActor, public Named
//{
//public:
//    CMan(std::wstring n): Named(n) {}
//
//    void hear(CSayEvent event)
//    {
//        std::wcout << name << L" услышал: " << event.m_data << std::endl;
//    }
//};
//
////женщина
//class CWoman: public event::CActor, public Named
//{
//public:
//    CWoman(std::wstring n): Named(n) {}
//
//    void hear(CSayEvent event)
//    {
//        std::wcout << name << L" услышала: " << event.m_data << std::endl;
//    }
//};
//
////ребенок
//class CBaby: public event::CActor, public Named
//{
//public:
//    CBaby(std::wstring n): Named(n) {}
//};
//
////-----------------------------------------------
//class A: public event::CActor
//{
//public:
//    void f(const std::string &name, const std::string &param)
//    {
//        std::cout<<"Name: "<<name<<" Param: "<<param<<"\n";
//    }
//};
//
////-----------------------------------------------
void main()
{
    //std::wcout.imbue(std::locale(".866"));

    //{
    //    CSpy spy1(L"Bond"),
    //         spy2(L"Kazanova");

    //    CMan man1(L"Adam"),
    //         man2(L"Superman"),
    //         man3(L"Romeo");

    //    CWoman woman1(L"Eva"),
    //           woman2(L"Juliet"),
    //           woman3(L"Anna");

    //    CBaby baby1(L"Kain"),
    //          baby2(L"Avel");

    //    //Бонд подслушивает все разговоры
    //    spy1.subscribe<CSayEvent>(&spy1, &CSpy::spy);

    //    //Казанова подслушивает только женщин
    //    spy2.subscribe<CSayEvent>(&spy2, &CSpy::spy, &woman1);
    //    spy2.subscribe<CSayEvent>(&spy2, &CSpy::spy, &woman2);
    //    spy2.subscribe<CSayEvent>(&spy2, &CSpy::spy, &woman3);

    //    //супермен слышит все!
    //    man2.subscribe<CSayEvent>(&man2, &CMan::hear);

    //    //Адам слышит только Еву
    //    man1.subscribe<CSayEvent>(&man1, &CMan::hear, &woman1);

    //    //Ева слышит только Адама и своих детей
    //    woman1.subscribe<CSayEvent>(&woman1, &CWoman::hear, &man1);
    //    woman1.subscribe<CSayEvent>(&woman1, &CWoman::hear, &baby1);
    //    woman1.subscribe<CSayEvent>(&woman1, &CWoman::hear, &baby2);

    //    //Джульетта и Анна спят и нифига не слышат
    //    //Ромео сошел с ума от любви к Джульетте и не обращает никакого внимания на остальных
    //    //Детям глубоко пофиг, что говорят вокруг

    //    //-----------------------------------
    //    //Ромео убивается по Джульетте
    //    man3.sendEvent(CSayEvent(L"О Джульетта! Выйди на балкон, а? Ну че те стоит-то?"));
    //    std::wcout<<std::endl<<std::endl;

    //    //Адам проголодался
    //    man1.sendEvent(CSayEvent(L"Жена! Хочу жрать!"));
    //    std::wcout<<std::endl<<std::endl;

    //    //Анна во сне мечтает о муже
    //    woman3.sendEvent(CSayEvent(L"Ну куда подевались все жентельмены?"));
    //    std::wcout<<std::endl<<std::endl;

    //    //Дети дерутся
    //    baby2.sendEvent(CSayEvent(L"Мама! А Каин меня обижает!"));
    //    std::wcout<<std::endl<<std::endl;
    //}

    //{
    //    A a1,a2,a3;

    //    a1.subscribe("SomeName1", &a1, &A::f);
    //    a2.subscribe("SomeName2", &a2, &A::f);

    //    a3.sendEvent("SomeName1","to a1");
    //    a3.sendEvent("SomeName2","to a2");
    //    a3.sendEvent("SomeName3","to nothing");
    //}

    //event::IEventManager::DeleteAllManagers();
}