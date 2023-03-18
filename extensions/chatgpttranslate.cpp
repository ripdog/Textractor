#include "qtcommon.h"
#include "translatewrapper.h"
#include "network.h"

extern const wchar_t* TRANSLATION_ERROR;

const char* TRANSLATION_PROVIDER = "ChatGPT Translate";
const char* GET_API_KEY_FROM = "https://platform.openai.com/account/api-keys";
extern const QStringList languagesTo
{
   "Afrikaans",
   "Albanian",
   "Amharic",
   "Arabic",
   "Armenian",
   "Azerbaijani",
   "Basque",
   "Belarusian",
   "Bengali",
   "Bosnian",
   "Bulgarian",
   "Catalan",
   "Cebuano",
   "Chichewa",
   "Chinese (Simplified)",
   "Chinese (Traditional)",
   "Corsican",
   "Croatian",
   "Czech",
   "Danish",
   "Dutch",
   "English",
   "Esperanto",
   "Estonian",
   "Filipino",
   "Finnish",
   "French",
   "Frisian",
   "Galician",
   "Georgian",
   "German",
   "Greek",
   "Gujarati",
   "Haitian Creole",
   "Hausa",
   "Hawaiian",
   "Hebrew",
   "Hindi",
   "Hmong",
   "Hungarian",
   "Icelandic",
   "Igbo",
   "Indonesian",
   "Irish",
   "Italian",
   "Japanese",
   "Javanese",
   "Kannada",
   "Kazakh",
   "Khmer",
   "Kinyarwanda",
   "Korean",
   "Kurdish (Kurmanji)",
   "Kyrgyz",
   "Lao",
   "Latin",
   "Latvian",
   "Lithuanian",
   "Luxembourgish",
   "Macedonian",
   "Malagasy",
   "Malay",
   "Malayalam",
   "Maltese",
   "Maori",
   "Marathi",
   "Mongolian",
   "Myanmar (Burmese)",
   "Nepali",
   "Norwegian",
   "Odia (Oriya)",
   "Pashto",
   "Persian",
   "Polish",
   "Portuguese",
   "Punjabi",
   "Romanian",
   "Russian",
   "Samoan",
   "Scots Gaelic",
   "Serbian",
   "Sesotho",
   "Shona",
   "Sindhi",
   "Sinhala",
   "Slovak",
   "Slovenian",
   "Somali",
   "Spanish",
   "Sundanese",
   "Swahili",
   "Swedish",
   "Tajik",
   "Tamil",
   "Tatar",
   "Telugu",
   "Thai",
   "Turkish",
   "Turkmen",
   "Ukrainian",
   "Urdu",
   "Uyghur",
   "Uzbek",
   "Vietnamese",
   "Welsh",
   "Xhosa",
   "Yiddish",
   "Yoruba",
   "Zulu",
}, languagesFrom = languagesTo;
extern const std::unordered_map<std::wstring, std::wstring> codes{};

bool translateSelectedOnly = true, useRateLimiter = true, rateLimitSelected = true, useCache = true, useFilter = true;
int tokenCount = 30, rateLimitTimespan = 60000, maxSentenceSize = 1000;

std::pair<bool, std::wstring> Translate(const std::wstring& text, TranslationParam tlp)
{
	if (!tlp.authKey.empty())
	{
		auto prompt = JSON::Escape(FormatString(R"(You are a translator. You will translate the received text into %s. You will return only the translated text, with no commentary. You will not repeat the input. Input: %s)", 
			WideStringToString(tlp.translateTo), WideStringToString(text)));
		auto body = FormatString(R"({"model": "gpt-3.5-turbo","messages": [{"role": "user", "content":"%s"}]})"
			, prompt);
		if (HttpRequest httpRequest{
			L"Textractor",
			L"api.openai.com",
			L"POST",
			L"/v1/chat/completions",
			body,
			FormatString(L"Authorization: Bearer %s \r\nContent-Type: application/json; charset=utf-8", tlp.authKey).c_str(),
			INTERNET_DEFAULT_PORT,
			L"",
			WINHTTP_FLAG_SECURE
			})
			if (auto translation = Copy(JSON::Parse(httpRequest.response)[L"choices"][0][L"message"][L"content"].String())) return { true, translation.value() };
			else return { false, FormatString(L"%s: %s", TRANSLATION_ERROR, httpRequest.response) };
		else return { false, FormatString(L"%s (code=%u)", TRANSLATION_ERROR, httpRequest.errorCode) };
	}

	else return { false, L" Please configure your OpenAI API key in ChatGPT translate Settings first." };
}
