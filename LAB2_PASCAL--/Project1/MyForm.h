#pragma once
#include <msclr/marshal_cppstd.h>
#include <regex>
#include <iostream>
#include <string>
#include <vector>
#include <variant>

#include "../Base/Token.h"
#include "../Interpreter/Interpreter.h"
#include "../Lexer/Lexer.h"
#include "../Parser/Parser.h"
#include "../ExpressionEvaluator/PostfixConverter.cpp"


namespace Project1 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	
	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();

			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}

	private: System::Windows::Forms::Button^ button1;
	private: System::Windows::Forms::RichTextBox^ richTextBox1;
	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->richTextBox1 = (gcnew System::Windows::Forms::RichTextBox());
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->button1->Font = (gcnew System::Drawing::Font(L"Consolas", 10.125F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->button1->Location = System::Drawing::Point(1401, 708);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(185, 106);
			this->button1->TabIndex = 1;
			this->button1->Text = L"Run";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
			// 
			// richTextBox1
			// 
			this->richTextBox1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->richTextBox1->Font = (gcnew System::Drawing::Font(L"Cascadia Mono", 24, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->richTextBox1->Location = System::Drawing::Point(56, 30);
			this->richTextBox1->Name = L"richTextBox1";
			this->richTextBox1->Size = System::Drawing::Size(1194, 540);
			this->richTextBox1->TabIndex = 0;
			this->richTextBox1->Text = L"";
			this->richTextBox1->TextChanged += gcnew System::EventHandler(this, &MyForm::richTextBox1_TextChanged);
			this->richTextBox1->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &MyForm::richTextBox1_KeyDown);
			this->richTextBox1->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &MyForm::richTextBox1_KeyUp);
			this->richTextBox1->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::richTextBox1_MouseWheel);
			this->richTextBox1->PreviewKeyDown += gcnew System::Windows::Forms::PreviewKeyDownEventHandler(this, &MyForm::richTextBox1_PreviewKeyDown);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(12, 25);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1792, 995);
			this->Controls->Add(this->richTextBox1);
			this->Controls->Add(this->button1);
			this->Name = L"MyForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"MyForm";
			this->WindowState = System::Windows::Forms::FormWindowState::Maximized;
			this->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &MyForm::MyForm_KeyDown);
			this->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &MyForm::MyForm_KeyUp);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: void HighlightKeyword(String^ keyword, Color color) {
		String^ pattern = "\\b" + keyword + "\\b"; // "\\b" - граница слова (word boundary)
		System::Text::RegularExpressions::Regex^ regex = gcnew System::Text::RegularExpressions::Regex(pattern);

		System::Text::RegularExpressions::MatchCollection^ matches = regex->Matches(richTextBox1->Text);

		for (int i = 0; i < matches->Count; ++i) {
			System::Text::RegularExpressions::Match^ match = matches[i];
			richTextBox1->SelectionStart = match->Index;
			richTextBox1->SelectionLength = match->Length;
			richTextBox1->SelectionColor = color;
		}
	}
	private: System::Void richTextBox1_TextChanged(System::Object^ sender, System::EventArgs^ e)
	{
		int selectionStart = richTextBox1->SelectionStart;
		int selectionLength = richTextBox1->SelectionLength;

		// Убираем выделение, чтобы не повлиять на форматирование
		richTextBox1->SelectAll();
		richTextBox1->SelectionColor = Color::Black; // Сбрасываем цвет
		richTextBox1->SelectionFont = gcnew System::Drawing::Font(richTextBox1->Font, FontStyle::Regular); // Сбрасываем стиль

		// Ключевые слова (пример)
		HighlightKeyword("program", Color::Orange);
		HighlightKeyword("begin", Color::Purple);
		HighlightKeyword("const", Color::Blue);
		HighlightKeyword("var", Color::Blue);
		HighlightKeyword("end", Color::Purple);
		HighlightKeyword("end.", Color::Blue);
		HighlightKeyword("Write", Color::SkyBlue);
		HighlightKeyword("Read", Color::SkyBlue);
		HighlightKeyword("if", Color::Red);
		HighlightKeyword("else", Color::Red);
		HighlightKeyword("then", Color::Red);
		HighlightKeyword("int", Color::DarkBlue);
		HighlightKeyword("double", Color::DarkBlue);
		HighlightKeyword("string", Color::DarkBlue);
		HighlightStringLiterals(Color::Green);
		//Восстанавливаем позицию курсора
		richTextBox1->SelectionStart = selectionStart;
		richTextBox1->SelectionLength = selectionLength;
		richTextBox1->SelectionColor = Color::Black;
	}
	private: void HighlightStringLiterals(Color color)
	{
		String^ pattern = "\"(.*?)\"";

		System::Text::RegularExpressions::Regex^ regex = gcnew System::Text::RegularExpressions::Regex(pattern);
		System::Text::RegularExpressions::MatchCollection^ matches = regex->Matches(richTextBox1->Text);

		for (int i = 0; i < matches->Count; ++i) {
			System::Text::RegularExpressions::Match^ match = matches[i];
			richTextBox1->SelectionStart = match->Index;
			richTextBox1->SelectionLength = match->Length;
			richTextBox1->SelectionColor = color;
		}
	}
	private: System::Void richTextBox1_PreviewKeyDown(System::Object^ sender, System::Windows::Forms::PreviewKeyDownEventArgs^ e) {
		if (e->KeyCode == Keys::Tab) {
			e->IsInputKey = true;
		}
	}
	private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e)
	{	
		try
		{
			//System::String^ text = textBox1->Text;
			System::String^ text = richTextBox1->Text;
			//MessageBox::Show(text);

			std::string PascalCode = msclr::interop::marshal_as<std::string>(text);

			Lexer lexer(PascalCode);

			const vector<Token> tokens = lexer.tokenize();

			Parser parser(tokens);
			auto& ast = parser.parse();

			Interpreter inter(ast);

			system("cls");
			inter.run();
		}
		catch (const std::exception& e)
		{
			System::String^ errorMessage = gcnew System::String(e.what());
			MessageBox::Show("Ошибка: " + errorMessage, "Ошибка программы", MessageBoxButtons::OK, MessageBoxIcon::Error);
		}
	}
		   

	private: bool ctrlKeyPressed = false;
	private: System::Void MyForm_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e)
	{
		if (e->KeyCode == Keys::ControlKey)
		{
			ctrlKeyPressed = true;
		}
	}
	private: System::Void MyForm_KeyUp(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e)
	{
		if (e->KeyCode == Keys::ControlKey)
		{
			ctrlKeyPressed = false;
		}
	}
	private: System::Void richTextBox1_MouseWheel(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
	{
		if (ctrlKeyPressed) {
			float currentSize = richTextBox1->Font->Size;
			float newSize = currentSize;

			// Увеличиваем размер шрифта, если колесико мыши прокручено вверх
			if (e->Delta > 0) {
				newSize += 1.0f; // Шаг изменения размера шрифта
			}
			// Уменьшаем размер шрифта, если колесико мыши прокручено вниз
			else {
				newSize -= 1.0f;
			}

			// Проверка на минимальный и максимальный размер шрифта (опционально)
			if (newSize < 6.0f) newSize = 6.0f;
			if (newSize > 72.0f) newSize = 72.0f;

			richTextBox1->Font = gcnew System::Drawing::Font(richTextBox1->Font->FontFamily, newSize, richTextBox1->Font->Style);

			// Предотвращаем прокрутку TextBox, чтобы прокручивался только шрифт
			//((HandledEventArgs^)e)->Handled = true;
		}
	}
	private: System::Void richTextBox1_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e)
	{
		if (e->KeyCode == Keys::ControlKey)
		{
			ctrlKeyPressed = true;
		}

		if (e->KeyCode == Keys::Tab)
		{
			richTextBox1->SelectedText = "\t";
			e->Handled = true;
			e->SuppressKeyPress = true;
		}
	}
	private: System::Void richTextBox1_KeyUp(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e)
	{
		if (e->KeyCode == Keys::ControlKey)
		{
			ctrlKeyPressed = false;
		}
	}
private: System::Void textBox1_TextChanged(System::Object^ sender, System::EventArgs^ e) {
}
};
}
