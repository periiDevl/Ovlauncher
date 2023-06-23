#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"

#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include <iostream>
#include <filesystem>
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

int main()
{
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

		

		
		
		ImGui::Begin("Hello 2");
		ImGui::Text("Hello");
		if (ImGui::Button("Button"))
		{
			printf("Hello0");

			fs::path source_folder = "/path/to/source/folder";
			fs::path destination_folder = "/path/to/destination/folder";

			copy_folder(source_folder, destination_folder);

		}
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