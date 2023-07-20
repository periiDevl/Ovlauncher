#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"

#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include <iostream>
#include <filesystem>
#include <thread> 
#include <fstream>
using namespace std;
namespace fs = std::filesystem;


void copy_folder(const fs::path& source_folder, const fs::path& destination_folder) {
	if (!fs::exists(source_folder)) {
		cout << "Source folder does not exist." << endl;
		return;
	}

	if (fs::exists(destination_folder)) {
		cout << "Destination folder already exists." << endl;
		return;
	}

	for (const auto& entry : fs::directory_iterator(source_folder)) {
		const fs::path current_path = entry.path();
		const fs::path new_path = destination_folder / current_path.filename();

		if (fs::is_directory(current_path)) {
			copy_folder(current_path, new_path);
		}
		else {
			fs::copy(current_path, new_path);
		}
	}
}

bool copyFile(const std::string& sourceFile, const std::string& destinationFile) {
	try {
		fs::copy_file(sourceFile, destinationFile, fs::copy_options::overwrite_existing);
		return true;
	}
	catch (const fs::filesystem_error& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return false;
	}
}

int main()
{
	std::string filePath = "MsbuildPATH.ov";

	ofstream outFile;
	outFile.open(filePath, ios::trunc);

	outFile << std::filesystem::current_path().string() + "\\Build\\MSBuild\\Current\\Bin\\Msbuild.exe";

	outFile.close();

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(700, 400, "OV-Launcher ~ BoringInnovators", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetWindowAttrib(window, GLFW_RESIZABLE, GLFW_FALSE);
	gladLoadGL();
	glViewport(0, 0, 800, 800);



	



	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");


	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.00f, 0.00f, 0.00f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		

		
		
		ImGui::Begin("Your Projects");


		static char Name[128] = "";

		ImGui::InputText("Project Name", Name, IM_ARRAYSIZE(Name));

		if (ImGui::Button("Create Project")) {
			filesystem::copy("Assets", "projects/Assets");

			const char* oldName = "projects/Assets";

			std::string newName = "projects/";
			newName += Name;

			int result = std::rename(oldName, newName.c_str());
		}

		ImGui::Separator();

		for (const auto& dir : std::filesystem::directory_iterator("projects")) {
			if (dir.is_directory()) {
				if (ImGui::Button(dir.path().filename().string().c_str())) {
					std::string sourcePath = "MsbuildPATH.ov";
					std::string destinationPath = "projects/" + dir.path().filename().string() + "/MsbuildPATH.ov";

					bool success = copyFile(sourcePath, destinationPath);

					std::string path = dir.path().string() + "\\builder.exe";
					std::replace(path.begin(), path.end(), '/', '\\');

					std::string currentPath = std::filesystem::current_path().string();

					std::string fullPath = currentPath + "\\" + path;

					std::cout << "Running " << fullPath << std::endl;

					std::string batchDir = dir.path().string();
					std::replace(batchDir.begin(), batchDir.end(), '/', '\\');
					std::string cdCommand = "cd /d \"" + batchDir + "\"";

					std::string command = cdCommand + " && " + fullPath;

					std::thread([command]() {
						std::system(command.c_str());
						}).detach();

				}
			}
		}




		/*
		if (ImGui::Button("Button"))
		{
			printf("Hello0");

			filesystem::copy("Fold", "er/Fold");

		}
		*/
		ImGui::End();
		
		

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
