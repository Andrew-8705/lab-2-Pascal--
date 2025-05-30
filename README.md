# Pascal--

## Cостав команды :elephant: :elephant: :elephant:
* [Балдин Андрей](https://github.com/Andrew-8705)
* [Левонычев Иван](https://github.com/fartekool)
* [Лукин Иван](https://github.com/elus10n)

# 1. Назначение 
Программная система представляет собой интегрированную среду разработки для упрощенного языка программирования Pascal--. 
Основной целью системы является обеспечение возможности написания, синтаксического контроля и выполнения программ на языке Pascal-- в единой среде.

# 2. Требования к системе
- **Редактирование текста программы**: Предоставление текстового редактора для ввода и редактирования кода программы на языке Pascal--.
- **Синтаксический контроль**:
  - Автоматическая проверка корректности синтаксиса введенного кода.
  - Вывод информативных сообщений об ошибках с указанием номера строки и позиции ошибки.
  - Обработка вложенных условных операторов (if-then-else).
- **Хранение текста программы**: Внутреннее представление текста программы в виде иерархического списка (например, дерево синтаксического разбора или абстрактное синтаксическое дерево).
- **Хранение переменных и констант**:
  - Ведение таблицы для хранения информации о объявленных переменных и константах.
  - Реализация таблицы на выбор: упорядоченная таблица, дерево поиска, хеш-таблица.
  - Хранение имени, типа и значения каждой переменной и константы.
- **Выполнение программы**:
  - Реализация интерпретатора для выполнения программ на языке Pascal--.
  - Преобразование арифметических и условных выражений в постфиксную форму для последующего вычисления.
  - Вычисление значений арифметических выражений, включающих операции сложения, вычитания, умножения, деления (/), целочисленного деления (div) и взятия остатка (mod).
  - Вычисление значений условных выражений, включающих операции сравнения (=, <>, <, >, <=, >=).
  - Поддержка операторов присваивания (:=), ввода (Read) и вывода (Write).
  - Обработка стандартных типов данных: integer (целые числа) и double (вещественные числа).
- **Интерфейс пользователя**:
  - Графический интерфейс пользователя (GUI) с окном для редактирования кода.
  - Кнопки или пункты меню для выполнения синтаксического анализа и запуска программы.
  - Отображение сообщений об ошибках в отдельном окне или панели.
  - Отображение результатов выполнения программы в отдельном окне или панели.

# 3. Технические требования
- Язык программирования: C++.
- Среда разработки: Visual Studio.
- Платформа: Windows.
- Инструменты тестирования: Google Test (gtest)


# Пример возможной программы:
```pascal
program Example;
const
    Pi: double = 3.1415926;
var
    num1, num2: integer;
    Res, d: double;
begin
    num1 := 5;
    Write("Введите четное целое число: ");
    Read(num2);
    Write("Введите вещественное число: ");
	Read(d);
    if (b mod 2 = 0) then
    begin
        Res := (num1 - num2 * 5 div 2) / (d * 2);
        Write("Результат = ", Res);
    end
    else
        Write("Неверный ввод");
end.
```

---

# Список объектов и основных алгоритмов

## Объекты (классы / структуры данных)
1. **Token**
	- Представляет лексему
	- Атрибуты:
		- type: Тип токена (например, KEYWORD, IDENTIFIER, OPERATOR, INTEGER_LITERAL, DOUBLE_LITERAL, STRING_LITERAL, SEPARATOR).
		- value: Строковое представление токена (например, "program", "Example", ":=", "5", "3.14", ";").
		- line: Номер строки в исходном коде, где был найден токен.
		- column: Позиция символа в строке, где был найден токен.
2. **Node**
	- Базовый класс для представления узлов синтаксического дерева.
   	- Атрибуты:
   		- type: Тип узла (NodeType enum). Например, PROGRAM_STATEMENT, ASSIGNMENT_STATEMENT, IF_STATEMENT, IDENTIFIER_LIST, CONST_SECTION, VAR_SECTION, BEGIN_SECTION).
3. Производные классы Node
	- ProgramNode: Представляет корневой узел программы.
		- Атрибуты: programName: Имя программы (std::string).
	- ConstSectionNode: Представляет секцию констант (const ...).
	- VarSectionNode: Представляет секцию переменных (var ...).
	- BeginSectionNode: Представляет основное тело программы (begin ... end).
	- ConstDeclarationNode: Представляет объявление константы (например, Pi: double = 3.14;).
		- Атрибуты:
			1) identifier: Имя константы (std::string).
			2) type: Тип константы (std::string, например, "integer", "double", "string").
			3) value: Значение константы (std::variant<int, double, std::string>).
	- VariableDeclarationNode: Представляет объявление переменной (например, a, b: integer;).
		- Атрибуты:
			1) identifierList: Указатель на узел IdentifierListNode.
			2) type: Тип переменных (std::string).
	- IdentifierListNode: Представляет список идентификаторов (например, a, b).
		- Атрибуты: identifiers: Список имен идентификаторов (std::list<std::string>).
	- AssignmentStatementNode: Представляет оператор присваивания (например, x := 5;).
		- Атрибуты:
			1) variableName: Имя переменной, которой присваивается значение (std::string).
			2) expression: Вектор токенов, представляющий выражение в правой части присваивания (std::vector<Token>). Примечание: Планируется преобразование этого выражения в постфиксную форму для вычисления.
	- WriteStatementNode: Представляет оператор вывода (Write(x);).
		- Атрибуты: expression: Вектор токенов, представляющий выражение для вывода (std::vector<Token>). Примечание: Аналогично AssignmentStatementNode, планируется вычисление.
	- ReadStatementNode: Представляет оператор ввода (Read(y);).
		- Атрибуты: identifierList: Указатель на узел IdentifierListNode, содержащий переменные для ввода.
	- IfStatementNode: Представляет условный оператор (if (condition) then ... else ...).
		- Атрибуты:
			1) condition: Вектор токенов, представляющий условие (std::vector<Token>). Примечание: Планируется преобразование в постфиксную форму для вычисления.
			2) thenStatement: Список узлов (std::list<Node*>), представляющих блок then.
   			3) elseStatement: Список узлов (std::list<Node*>), представляющих блок else (может быть пустым).
4. **Abstract Syntax Tree (AST)**
   	- Представлен как std::list<std::list<Node*>>.
	- Внешний список содержит блоки верхнего уровня программы (например, ProgramNode в первом блоке, затем ConstSectionNode с объявлениями, VarSectionNode с объявлениями, BeginSectionNode с операторами).
	- Внутренние списки содержат последовательность узлов внутри каждого блока.

6. **SymbolTableRecord**
   	- Представляет запись в таблице переменных.
   	- Атрибуты:
   	  	- name: Имя переменной или константы (строка).
   	  	- type: Тип данных (integer, double).
   	  	- kind: Тип символа (constant, variable).
   	  	- value: std::variant<double, int> - Значение константы (если это константа).
7. **SymbolTable**
   	- Представляет таблицу переменных.
   	- Структура данных для хранения SymbolTableRecord (например, std::map<std::string, SymbolTableRecord>).
   	- Методы:
   	  	- insert(SymbolTableRecord record): Добавить запись в таблицу.
		- find(std::string name): Найти запись по имени.
		- update(std::string name, std::variant<double, int> newValue): Обновить значение переменной.
8. **PostfixExpression**
   	- Представляет выражение в постфиксной форме (может быть просто std::vector<Token> или более сложная структура).

## Основные алгоритмы (функции / методы)
1. **Лексический анализ** (Tokenizer)
   - tokenize(std::string sourceCode): Принимает исходный код и возвращает вектор объектов Token.
   - Алгоритм: Последовательное чтение исходного кода, распознавание лексем на основе правил языка Pascal-- (регулярные выражения или конечные автоматы), создание объектов Token для каждой найденной лексемы.
2. **Синтаксический анализ** (Parser)
   - parse(std::vector<Token> tokens): Принимает вектор токенов и строит AST (std::list<std::list<Node*>>).
   - Алгоритм: Рекурсивный спуск на основе грамматики языка Pascal--. Для каждого синтаксического правила создаются соответствующие узлы AST, устанавливаются связи между ними (в данном случае, путем добавления узлов в текущий блок std::list<Node*>). Обнаружение и обработка синтаксических ошибок (выброс исключений). Методы parseProgram, parseConstDeclarations, parseVarDeclarations, parseBeginStatement, parseStatement и другие вспомогательные методы реализуют правила грамматики.
3. **Управление таблицей переменных**
   - insert(SymbolTableRecord record): Добавление новой переменной или константы в таблицу.
   - find(std::string name): Поиск информации о переменной или константе по ее имени.
   - update(std::string name, ...): Обновление значения переменной во время выполнения.
4. **Семантический анализ** (SemanticAnalyzer)
   - analyze(Node* root, SymbolTable& symbolTable): Принимает иерархический список и таблицу переменных, выполняет семантические проверки.
   - Алгоритм: Обход иерархического списка, проверка типов, проверка объявлений, проверка областей видимости с использованием информации из таблицы переменных.
5. **Преобразование в постфиксную форму** (PostfixConverter)
   - convertToPostfix(Node* expressionNode): Принимает поддерево иерархического списка, представляющее выражение, и возвращает PostfixExpression.
   - Алгоритм: Алгоритм "сортировочной станции" с построением постфиксной записи.
6. **Вычисление постфиксных выражений** (PostfixCalculator)
   - calculate(PostfixExpression postfix, SymbolTable& symbolTable): Принимает постфиксное выражение и таблицу переменных, возвращает результат вычисления.
   - Алгоритм: Использование стека. При встрече операнда - поместить в стек, при встрече оператора - извлечь необходимое количество операндов из стека, выполнить операцию, поместить результат обратно в стек.
7. **Интерпретация и выполнение** (Interpreter)
   - execute(Node* root, SymbolTable& symbolTable, OutputHandler& output): Принимает иерархический список, таблицу переменных и объект для вывода, выполняет программу.
   - Алгоритм: Рекурсивный обход иерархического списка. Для каждого типа узла (оператора) выполняется соответствующее действие (например, присваивание - обновление таблицы переменных, Write - вывод, Read - ввод, if - проверка условия и переход к соответствующей ветви, вычисление выражений с помощью PostfixCalculator).
8. **Обработка ошибок** (ErrorHandler)
   - reportError(std::string message, int line, int column): Выводит сообщение об ошибке пользователю. Используется на всех этапах.
		
