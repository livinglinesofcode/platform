#include <angelscript.h>
#include <scriptstdstring/scriptstdstring.h>
#include <assert.h>
#include <iostream>

void log(const std::string &msg) {
	std::cout << msg << std::endl;
}

void test() {
	asIScriptEngine* engine = asCreateScriptEngine();
	assert(engine);
	RegisterStdString(engine);

	int r = engine->RegisterGlobalFunction(
			"void log(const string &in)",
			asFUNCTION((void(*)(const std::string &msg))log),
			asCALL_CDECL);
	if (r < 0) {
		std::cerr << r << std::endl;
		return;
	}

	asIScriptModule* mod = engine->GetModule("TestModule", asGM_ALWAYS_CREATE);
	mod->AddScriptSection("script", "void main() { log(\"hello angelscript!\");  }");
	r = mod->Build();
	assert(r >= 0);

	asIScriptContext* ctx = engine->CreateContext();
	ctx->Prepare(mod->GetFunctionByName("main"));
	r = ctx->Execute();
	assert(r == asEXECUTION_FINISHED);

	ctx->Release();
	engine->ShutDownAndRelease();
}
