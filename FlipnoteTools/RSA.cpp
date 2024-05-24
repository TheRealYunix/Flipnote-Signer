#include <iostream>
#include <rsa.h>
#include <base64.h>
#include <hex.h>
#include <osrng.h.>
#include <files.h>
#include <cassert>
#include <sstream>
#include <windows.h>

namespace RSA
{
	using namespace CryptoPP;
	std::string SignString(std::string &message)
	{
		std::string RSA_PRIV_KEY =
			"MIICXAIBAAKBgQDCPLwTL6oSflv+gjywi/sM0TUB90xqOvuCpjduETjPoN2FwMeb\n"
			"xNjdKIqHUyDu4AvrQ6BDJc6gKUbZ1E27BGZoCPH49zQRb+zAM6M9EjHwQ6BABr0u\n"
			"2TcF7xGg2uQ9MBWz9AfbVQ91NjfrNWo0f7UPmffv1VvixmTk1BCtavZxBwIDAQAB\n"
			"AoGAJqdTfjX0EG4Y/JMKZM/Wi/xuIhAGovVN6/gL+9lthtQqgV2V24fW6FwTBU8j\n"
			"tKXdeVoh4Hr7nZ/ZO6wmM9tyTVSHo37XdF/1bsPM7iu/0M8A6+jJr94l6PYpCP5y\n"
			"apPEj2RR154gTaVK7/J/OePZy6tRlgcI1awsqgCDcvweEsECQQD4KnxSdyJD0Oqa\n"
			"xaIIhLkLZEuvkBmrLG+YQUezVOB60MjMC1DyIiZzc5otkQZ0RiopRm/57a6pXA58\n"
			"1xSN9JiRAkEAyF5uvKCbA9+PORUn668W0N+uRWZ2+WaqC46HkUDnrVEa4FsnLsFD\n"
			"0NlVmH2BXifPUPLMR6WzP+OJ9hKfve78FwJAQQJgLvomb716t7CuEa0zDFjpusP0\n"
			"9XJeiXQQZFoHtCSddVZBjiyEBhpyeR1Uo4D96nIZQ0+QQa1r3ig5qjY5AQJAWvZD\n"
			"324p8YA0TP3FucEq4ngpbWgu6tooqEZ0VQTaKFyBjwjSqO8kElQX/7o6WLxJ6b3P\n"
			"71bSIVby8rtRubAc0QJBAJ1zhk7/d6PA4+J9aQ+jBi/OW+ljiD8yjatLq+4fqRfx\n"
			"YsFzLqmzjxfiiX+6BzuHHcWJzBUmIcPyu+Y2N0BdT7M=\n";

		CryptoPP::ByteQueue queue;
		CryptoPP::Base64Decoder decoder;

		decoder.Attach(new CryptoPP::Redirector(queue));
		decoder.Put((const CryptoPP::byte*)RSA_PRIV_KEY.data(), RSA_PRIV_KEY.length());
		decoder.MessageEnd();

		try
		{
			CryptoPP::RSA::PrivateKey rsaPrivate;
			rsaPrivate.BERDecodePrivateKey(queue, false /*paramsPresent*/, queue.MaxRetrievable());

			assert(queue.IsEmpty());

			CryptoPP::AutoSeededRandomPool prng;
			bool valid = rsaPrivate.Validate(prng, 3);
			if (!valid)
				std::cerr << "RSA private key is not valid" << std::endl;

			/*
			std::cout << "N:" << rsaPrivate.GetModulus() << std::endl;
			std::cout << "E:" << rsaPrivate.GetPublicExponent() << std::endl;
			std::cout << "D:" << rsaPrivate.GetPrivateExponent() << std::endl;
			*/

			AutoSeededRandomPool rng;

			// sign message
			std::string signature;
			RSASS<PKCS1v15, SHA1>::Signer signer(rsaPrivate);

			CryptoPP::StringSource ss(message, true,
				new CryptoPP::SignerFilter(rng, signer,
					new CryptoPP::HexEncoder(
						new CryptoPP::StringSink(signature))));

			return signature;

		}
		catch (const Exception& ex)
		{
			std::cerr << ex.what() << std::endl;
			exit(1);
		}
	}

	bool Sign(const char* filename)
	{
		std::ifstream input(filename, std::ios::binary);
		if (!input.is_open())
		{
			std::cerr << "Error: Unable to open file: " << filename << std::endl;
			return false;
		}

		std::string str(std::istreambuf_iterator<char>(input), {});
		input.close();

		if (str.empty())
		{
			std::cerr << "Error: Unable to read file content: " << filename << std::endl;
			return false;
		}

		str.erase(str.size() - 0x90);
		std::string Hash = SignString(str);

		std::ofstream File(filename, std::ios::out | std::ios::in | std::ios::binary);
		if (!File.is_open())
		{
			std::cerr << "Error: Unable to open file for writing: " << filename << std::endl;
			return false;
		}

		std::basic_string<uint8_t> bytes;
		for (size_t i = 0; i < Hash.length(); i += 2)
		{
			uint16_t byte;
			std::string nextbyte = Hash.substr(i, 2);
			std::istringstream(nextbyte) >> std::hex >> byte;
			bytes.push_back(static_cast<uint8_t>(byte));
		}

		File.seekp(-0x90, std::ios::end);
		File.write((char *)bytes.data(), bytes.size());

		if (!File)
		{
			std::cerr << "Error: Unable to write signature to file: " << filename << std::endl;
			return false;
		}
		std::cout << "File successfully signed: " << filename << std::endl;
		return true;
	}

	std::string string_to_hex(const std::string& input)
	{
		static const char hex_digits[] = "0123456789ABCDEF";

		std::string output;
		output.reserve(input.length() * 2);
		for (unsigned char c : input)
		{
			output.push_back(hex_digits[c >> 4]);
			output.push_back(hex_digits[c & 15]);
		}
		return output;
	}

	std::string ReadBinary(const char *filename, uint32_t size, int seek, int SeekFlag)
	{
		std::ifstream File(filename, std::ios::in | std::ios::binary);

		std::vector<char> buffer(size);

		File.seekg(seek, SeekFlag);
		File.read(buffer.data(), size);

		std::string Buf;

		for (int i = 0; i < size; i++)
		{
			Buf += buffer[i];
		}
		File.close();

		return string_to_hex(Buf);
	}

	bool CheckSign(const char* filename)
	{
			std::ifstream input(filename, std::ios::binary);
			std::string str(std::istreambuf_iterator<char>(input), {});
			str.erase(str.size() - 0x90);
			std::string OriginalHash;
			OriginalHash = SignString(str);
			input.close();
			if (ReadBinary(filename, 0x80, -0x90, std::ios::end) != OriginalHash)
				return false;
			else
				return true;

	}
};