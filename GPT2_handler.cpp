#include <boost/python.hpp>
#include "GPT2_handler.h"

Gpt2Model::Gpt2Model(const std::string& modelPath) : model_path(modelPath) {
    Py_Initialize();  // Инициализация Python

    try {
        boost::python::object main = boost::python::import("__main__");
        boost::python::object global(main.attr("__dict__"));

        // Загрузка модели GPT-2
        boost::python::exec(boost::python::str(
            "import gpt_2_simple as gpt2\n"
            "model_name = '"+ model_path +"'\n"
            "sess = gpt2.start_tf_sess()\n"
            "gpt2.load_gpt2(sess, model_name=model_name)\n"
        ), global);
    } catch (boost::python::error_already_set const &) {
        PyErr_Print();
    }
}

std::string Gpt2Model::generateResponse(const std::string& inputText) {
    try {
        boost::python::object main = boost::python::import("__main__");
        boost::python::object global(main.attr("__dict__"));

        // Вызов Python-скрипта и передача ввода
        boost::python::exec(boost::python::str(
            "text = gpt2.generate(sess, model_name='"+ model_path +"', prefix='" + inputText + "', length=250, return_as_list=True)[0]\n"
        ), global);

        return boost::python::extract<std::string>(global["text"]);
    } catch (boost::python::error_already_set const &) {
        PyErr_Print();
        return "Ошибка при генерации ответа";
    }
}
