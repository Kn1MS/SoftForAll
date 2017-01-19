#pragma once

namespace TextEditor {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
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
	private: System::Windows::Forms::RichTextBox^  richTextBox1;
	protected:
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::CheckBox^  checkBox1;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::Button^  button3;
	private: System::Windows::Forms::ContextMenuStrip^  contextMenuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  canselToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  undoToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  cutToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  copyToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  pasteToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  deleteToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  allToolStripMenuItem;
	private: Crad::Windows::Forms::Actions::ActionList^  actionList1;
	private: Crad::Windows::Forms::Actions::CopyAction^  copyAction1;
	private: Crad::Windows::Forms::Actions::CutAction^  cutAction1;
	private: Crad::Windows::Forms::Actions::DeleteAction^  deleteAction1;
	private: Crad::Windows::Forms::Actions::PasteAction^  pasteAction1;
	private: Crad::Windows::Forms::Actions::UndoAction^  undoAction1;
	private: Crad::Windows::Forms::Actions::SelectAllAction^  selectAllAction1;
	private: Crad::Windows::Forms::Actions::RedoAction^  redoAction1;
	private: System::Windows::Forms::SaveFileDialog^  saveFileDialog1;
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;

	private: System::ComponentModel::IContainer^  components;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(MyForm::typeid));
			this->richTextBox1 = (gcnew System::Windows::Forms::RichTextBox());
			this->contextMenuStrip1 = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->canselToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->undoToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->cutToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->copyToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->pasteToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->deleteToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->allToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->checkBox1 = (gcnew System::Windows::Forms::CheckBox());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->actionList1 = (gcnew Crad::Windows::Forms::Actions::ActionList());
			this->undoAction1 = (gcnew Crad::Windows::Forms::Actions::UndoAction());
			this->redoAction1 = (gcnew Crad::Windows::Forms::Actions::RedoAction());
			this->cutAction1 = (gcnew Crad::Windows::Forms::Actions::CutAction());
			this->copyAction1 = (gcnew Crad::Windows::Forms::Actions::CopyAction());
			this->pasteAction1 = (gcnew Crad::Windows::Forms::Actions::PasteAction());
			this->deleteAction1 = (gcnew Crad::Windows::Forms::Actions::DeleteAction());
			this->selectAllAction1 = (gcnew Crad::Windows::Forms::Actions::SelectAllAction());
			this->saveFileDialog1 = (gcnew System::Windows::Forms::SaveFileDialog());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->contextMenuStrip1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->actionList1))->BeginInit();
			this->SuspendLayout();
			// 
			// richTextBox1
			// 
			this->richTextBox1->ContextMenuStrip = this->contextMenuStrip1;
			this->richTextBox1->Location = System::Drawing::Point(12, 12);
			this->richTextBox1->Name = L"richTextBox1";
			this->richTextBox1->Size = System::Drawing::Size(760, 539);
			this->richTextBox1->TabIndex = 0;
			this->richTextBox1->Text = L"";
			this->richTextBox1->WordWrap = false;
			// 
			// contextMenuStrip1
			// 
			this->contextMenuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(7) {
				this->canselToolStripMenuItem,
					this->undoToolStripMenuItem, this->cutToolStripMenuItem, this->copyToolStripMenuItem, this->pasteToolStripMenuItem, this->deleteToolStripMenuItem,
					this->allToolStripMenuItem
			});
			this->contextMenuStrip1->Name = L"contextMenuStrip1";
			this->contextMenuStrip1->Size = System::Drawing::Size(210, 158);
			// 
			// canselToolStripMenuItem
			// 
			this->actionList1->SetAction(this->canselToolStripMenuItem, this->undoAction1);
			this->canselToolStripMenuItem->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"canselToolStripMenuItem.Image")));
			this->canselToolStripMenuItem->Name = L"canselToolStripMenuItem";
			this->canselToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::Z));
			this->canselToolStripMenuItem->Size = System::Drawing::Size(209, 22);
			this->canselToolStripMenuItem->Text = L"&Undo";
			this->canselToolStripMenuItem->ToolTipText = L"Undo";
			// 
			// undoToolStripMenuItem
			// 
			this->actionList1->SetAction(this->undoToolStripMenuItem, this->redoAction1);
			this->undoToolStripMenuItem->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"undoToolStripMenuItem.Image")));
			this->undoToolStripMenuItem->Name = L"undoToolStripMenuItem";
			this->undoToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::Y));
			this->undoToolStripMenuItem->Size = System::Drawing::Size(209, 22);
			this->undoToolStripMenuItem->Text = L"&Redo";
			this->undoToolStripMenuItem->ToolTipText = L"Redo";
			// 
			// cutToolStripMenuItem
			// 
			this->actionList1->SetAction(this->cutToolStripMenuItem, this->cutAction1);
			this->cutToolStripMenuItem->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"cutToolStripMenuItem.Image")));
			this->cutToolStripMenuItem->Name = L"cutToolStripMenuItem";
			this->cutToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::X));
			this->cutToolStripMenuItem->Size = System::Drawing::Size(209, 22);
			this->cutToolStripMenuItem->Text = L"Cu&t";
			this->cutToolStripMenuItem->ToolTipText = L"Cut";
			// 
			// copyToolStripMenuItem
			// 
			this->actionList1->SetAction(this->copyToolStripMenuItem, this->copyAction1);
			this->copyToolStripMenuItem->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"copyToolStripMenuItem.Image")));
			this->copyToolStripMenuItem->Name = L"copyToolStripMenuItem";
			this->copyToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::C));
			this->copyToolStripMenuItem->Size = System::Drawing::Size(209, 22);
			this->copyToolStripMenuItem->Text = L"&Copy";
			this->copyToolStripMenuItem->ToolTipText = L"Copy";
			// 
			// pasteToolStripMenuItem
			// 
			this->actionList1->SetAction(this->pasteToolStripMenuItem, this->pasteAction1);
			this->pasteToolStripMenuItem->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pasteToolStripMenuItem.Image")));
			this->pasteToolStripMenuItem->Name = L"pasteToolStripMenuItem";
			this->pasteToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::V));
			this->pasteToolStripMenuItem->Size = System::Drawing::Size(209, 22);
			this->pasteToolStripMenuItem->Text = L"&Paste";
			this->pasteToolStripMenuItem->ToolTipText = L"Paste";
			// 
			// deleteToolStripMenuItem
			// 
			this->actionList1->SetAction(this->deleteToolStripMenuItem, this->deleteAction1);
			this->deleteToolStripMenuItem->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"deleteToolStripMenuItem.Image")));
			this->deleteToolStripMenuItem->Name = L"deleteToolStripMenuItem";
			this->deleteToolStripMenuItem->ShortcutKeys = System::Windows::Forms::Keys::Delete;
			this->deleteToolStripMenuItem->Size = System::Drawing::Size(209, 22);
			this->deleteToolStripMenuItem->Text = L"&Delete";
			this->deleteToolStripMenuItem->ToolTipText = L"Delete";
			// 
			// allToolStripMenuItem
			// 
			this->actionList1->SetAction(this->allToolStripMenuItem, this->selectAllAction1);
			this->allToolStripMenuItem->Name = L"allToolStripMenuItem";
			this->allToolStripMenuItem->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::NumPad5));
			this->allToolStripMenuItem->Size = System::Drawing::Size(209, 22);
			this->allToolStripMenuItem->Text = L"Select &All";
			this->allToolStripMenuItem->ToolTipText = L"Select All";
			// 
			// button1
			// 
			this->button1->Cursor = System::Windows::Forms::Cursors::Hand;
			this->button1->Location = System::Drawing::Point(12, 554);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 1;
			this->button1->Text = L"Clear";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
			// 
			// checkBox1
			// 
			this->checkBox1->AutoSize = true;
			this->checkBox1->Cursor = System::Windows::Forms::Cursors::Hand;
			this->checkBox1->Location = System::Drawing::Point(93, 557);
			this->checkBox1->Name = L"checkBox1";
			this->checkBox1->Size = System::Drawing::Size(83, 19);
			this->checkBox1->TabIndex = 2;
			this->checkBox1->Text = L"Read Only";
			this->checkBox1->UseVisualStyleBackColor = true;
			this->checkBox1->CheckedChanged += gcnew System::EventHandler(this, &MyForm::checkBox1_CheckedChanged);
			// 
			// button2
			// 
			this->button2->Cursor = System::Windows::Forms::Cursors::Hand;
			this->button2->Location = System::Drawing::Point(566, 552);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(100, 23);
			this->button2->TabIndex = 3;
			this->button2->Text = L"Load from...";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &MyForm::button2_Click);
			// 
			// button3
			// 
			this->button3->Cursor = System::Windows::Forms::Cursors::Hand;
			this->button3->Location = System::Drawing::Point(672, 553);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(100, 23);
			this->button3->TabIndex = 4;
			this->button3->Text = L"Save As...";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &MyForm::button3_Click);
			// 
			// actionList1
			// 
			this->actionList1->Actions->Add(this->copyAction1);
			this->actionList1->Actions->Add(this->cutAction1);
			this->actionList1->Actions->Add(this->deleteAction1);
			this->actionList1->Actions->Add(this->pasteAction1);
			this->actionList1->Actions->Add(this->undoAction1);
			this->actionList1->Actions->Add(this->selectAllAction1);
			this->actionList1->Actions->Add(this->redoAction1);
			this->actionList1->ContainerControl = this;
			// 
			// undoAction1
			// 
			this->undoAction1->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"undoAction1.Image")));
			this->undoAction1->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::Z));
			this->undoAction1->Text = L"&Undo";
			this->undoAction1->ToolTipText = L"Undo";
			// 
			// redoAction1
			// 
			this->redoAction1->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"redoAction1.Image")));
			this->redoAction1->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::Y));
			this->redoAction1->Text = L"&Redo";
			this->redoAction1->ToolTipText = L"Redo";
			// 
			// cutAction1
			// 
			this->cutAction1->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"cutAction1.Image")));
			this->cutAction1->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::X));
			this->cutAction1->Text = L"Cu&t";
			this->cutAction1->ToolTipText = L"Cut";
			// 
			// copyAction1
			// 
			this->copyAction1->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"copyAction1.Image")));
			this->copyAction1->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::C));
			this->copyAction1->Text = L"&Copy";
			this->copyAction1->ToolTipText = L"Copy";
			// 
			// pasteAction1
			// 
			this->pasteAction1->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pasteAction1.Image")));
			this->pasteAction1->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::V));
			this->pasteAction1->Text = L"&Paste";
			this->pasteAction1->ToolTipText = L"Paste";
			// 
			// deleteAction1
			// 
			this->deleteAction1->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"deleteAction1.Image")));
			this->deleteAction1->ShortcutKeys = System::Windows::Forms::Keys::Delete;
			this->deleteAction1->Text = L"&Delete";
			this->deleteAction1->ToolTipText = L"Delete";
			// 
			// selectAllAction1
			// 
			this->selectAllAction1->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::NumPad5));
			this->selectAllAction1->Text = L"Select &All";
			this->selectAllAction1->ToolTipText = L"Select All";
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(7, 15);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(784, 586);
			this->Controls->Add(this->button3);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->checkBox1);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->richTextBox1);
			this->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->MaximizeBox = false;
			this->MinimumSize = System::Drawing::Size(800, 625);
			this->Name = L"MyForm";
			this->ShowIcon = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"TextEditor";
			this->contextMenuStrip1->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->actionList1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}

#pragma endregion
		private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
			//Очиста поля.
			richTextBox1->Clear();
		}
	
		private: System::Void checkBox1_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			//Изменение параметра "Только чтение" поля
			if (richTextBox1->ReadOnly == true)
				richTextBox1->ReadOnly = false;
			else
				richTextBox1->ReadOnly = true;
		}

		private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
			Stream^ myStream;
			//OpenFileDialog^ openFileDialog1 = gcnew OpenFileDialog;

			openFileDialog1->InitialDirectory = "C:\\";
			openFileDialog1->Filter = "txt files (*.txt)|*.txt|All files (*.*)|*.*";
			openFileDialog1->FilterIndex = 2;
			openFileDialog1->RestoreDirectory = true;

			if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
				if ((myStream = openFileDialog1->OpenFile()) != nullptr) {
					try {
						// Загрузка открытого файла в поле.
						System::Text::Encoding^ defaul = System::Text::Encoding::Default;
						System::String^ readFile = System::IO::File::ReadAllText(openFileDialog1->FileName, defaul);
						richTextBox1->Text = readFile;
						myStream->Close();
					}
					catch (...) {
						// Если не удалось открыть файл, выводится ошибка.
						MessageBox::Show("ER01: Could not read file from disk.");
					}
				}
			}
		}

		private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) {
			Stream^ myStream;

			saveFileDialog1->Filter = "txt files (*.txt)|*.txt|All files (*.*)|*.*";
			saveFileDialog1->FilterIndex = 2;
			saveFileDialog1->RestoreDirectory = true;
			if (saveFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
				if ((myStream = saveFileDialog1->OpenFile()) != nullptr) {
					try {
						// Сохранение файла в указанный пользователем файл.
						richTextBox1->SaveFile(myStream, RichTextBoxStreamType::PlainText);
						myStream->Close();
					}
					catch (...) {
						// Если не получилось сохранить файл, выводится ошибка.
						MessageBox::Show("ER02: Could not write file to disk. ");
					}
				}
			}
		}
	};
}
