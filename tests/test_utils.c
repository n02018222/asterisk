/*
 * Asterisk -- An open source telephony toolkit.
 *
 * Copyright (C) 2010, Digium, Inc.
 *
 * David Vossel <dvossel@digium.com>
 * Russell Bryant <russell@digium.com>
 *
 * See http://www.asterisk.org for more information about
 * the Asterisk project. Please do not directly contact
 * any of the maintainers of this project for assistance;
 * the project provides a web site, mailing lists and IRC
 * channels for your use.
 *
 * This program is free software, distributed under the terms of
 * the GNU General Public License Version 2. See the LICENSE file
 * at the top of the source tree.
 */

/*!
 * \file
 * \brief Unit Tests for utils API
 *
 * \author David Vossel <dvossel@digium.com>
 * \author Russell Bryant <russell@digium.com>
 */

/*** MODULEINFO
	<depend>TEST_FRAMEWORK</depend>
 ***/

#include "asterisk.h"

ASTERISK_FILE_VERSION(__FILE__, "$Revision$");

#include "asterisk/utils.h"
#include "asterisk/test.h"
#include "asterisk/module.h"

AST_TEST_DEFINE(uri_encode_decode_test)
{
	int res = AST_TEST_PASS;
	const char *in = "abcdefghijklmnopurstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ 1234567890 ~`!@#$%^&*()_-+={[}]|\\:;\"'<,>.?/";
	const char *expected1 = "abcdefghijklmnopurstuvwxyz%20ABCDEFGHIJKLMNOPQRSTUVWXYZ%201234567890%20~%60!%40%23%24%25%5E%26*()_-%2B%3D%7B%5B%7D%5D%7C%5C%3A%3B%22'%3C%2C%3E.%3F%2F";
	const char *expected2 = "abcdefghijklmnopurstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ 1234567890 ~`!@#$%25^&*()_-+={[}]|\\:;\"'<,>.?/";
	char out[256] = { 0 };

	switch (cmd) {
	case TEST_INIT:
		info->name = "uri_encode_decode_test";
		info->category = "main/utils/";
		info->summary = "encode and decode a hex escaped string";
		info->description = "encode a string, verify encoded string matches what we expect.  Decode the encoded string, verify decoded string matches the original string.";
		return AST_TEST_NOT_RUN;
	case TEST_EXECUTE:
		break;
	}

	ast_test_status_update(test, "Input before executing ast_uri_encode:\n%s\n", in);
	ast_test_status_update(test, "Output expected for ast_uri_encode with enabling do_special_char: %s\n", expected1);
	ast_test_status_update(test, "Output expected for ast_uri_encode with out enabling do_special_char: %s\n\n", expected2);

	/* Test with do_special_char enabled */
	ast_uri_encode(in, out, sizeof(out), 1);
	ast_test_status_update(test, "Output after enabling do_special_char:\n%s\n", out);
	if (strcmp(expected1, out)) {
		ast_test_status_update(test, "ENCODE DOES NOT MATCH EXPECTED, FAIL\n");
		res = AST_TEST_FAIL;
	}

	/* Verify uri decode matches original */
	ast_uri_decode(out);
	if (strcmp(in, out)) {
		ast_test_status_update(test, "Decoded string did not match original input\n");
		res = AST_TEST_FAIL;
	} else {
		ast_test_status_update(test, "Decoded string matched original input\n");
	}

	/* Test with do_special_char disabled */
	out[0] = '\0';
	ast_uri_encode(in, out, sizeof(out), 0);
	ast_test_status_update(test, "Output after disabling do_special_char: %s\n", out);
	if (strcmp(expected2, out)) {
		ast_test_status_update(test, "ENCODE DOES NOT MATCH EXPECTED, FAIL\n");
		res = AST_TEST_FAIL;
	}

	/* Verify uri decode matches original */
	ast_uri_decode(out);
	if (strcmp(in, out)) {
		ast_test_status_update(test, "Decoded string did not match original input\n");
		res = AST_TEST_FAIL;
	} else {
		ast_test_status_update(test, "Decoded string matched original input\n");
	}

	return res;
}

AST_TEST_DEFINE(md5_test)
{
	static const struct {
		const char *input;
		const char *expected_output;
	} tests[] = {
		{ "apples",                          "daeccf0ad3c1fc8c8015205c332f5b42" },
		{ "bananas",                         "ec121ff80513ae58ed478d5c5787075b" },
		{ "reallylongstringaboutgoatcheese", "0a2d9280d37e2e37545cfef6e7e4e890" },
	};
	enum ast_test_result_state res = AST_TEST_PASS;
	int i;

	switch (cmd) {
	case TEST_INIT:
		info->name = "md5_test";
		info->category = "main/utils/";
		info->summary = "MD5 test";
		info->description =
			"This test exercises MD5 calculations."
			"";
		return AST_TEST_NOT_RUN;
	case TEST_EXECUTE:
		break;
	}

	ast_test_status_update(test, "Testing MD5 ...\n");

	for (i = 0; i < ARRAY_LEN(tests); i++) {
		char md5_hash[32];
		ast_md5_hash(md5_hash, tests[i].input);
		if (strcasecmp(md5_hash, tests[i].expected_output)) {
			ast_test_status_update(test,
					"input: '%s'  hash: '%s'  expected hash: '%s'\n",
					tests[i].input, md5_hash, tests[i].expected_output);
			res = AST_TEST_FAIL;
		}
	}

	return res;
}

AST_TEST_DEFINE(sha1_test)
{
	static const struct {
		const char *input;
		const char *expected_output;
	} tests[] = {
		{ "giraffe",
			"fac8f1a31d2998734d6a5253e49876b8e6a08239" },
		{ "platypus",
			"1dfb21b7a4d35e90d943e3a16107ccbfabd064d5" },
		{ "ParastratiosphecomyiaStratiosphecomyioides",
			"58af4e8438676f2bd3c4d8df9e00ee7fe06945bb" },
	};
	enum ast_test_result_state res = AST_TEST_PASS;
	int i;

	switch (cmd) {
	case TEST_INIT:
		info->name = "sha1_test";
		info->category = "main/utils/";
		info->summary = "SHA1 test";
		info->description =
			"This test exercises SHA1 calculations."
			"";
		return AST_TEST_NOT_RUN;
	case TEST_EXECUTE:
		break;
	}

	ast_test_status_update(test, "Testing SHA1 ...\n");

	for (i = 0; i < ARRAY_LEN(tests); i++) {
		char sha1_hash[64];
		ast_sha1_hash(sha1_hash, tests[i].input);
		if (strcasecmp(sha1_hash, tests[i].expected_output)) {
			ast_test_status_update(test,
					"input: '%s'  hash: '%s'  expected hash: '%s'\n",
					tests[i].input, sha1_hash, tests[i].expected_output);
			res = AST_TEST_FAIL;
		}
	}

	return res;
}

AST_TEST_DEFINE(base64_test)
{
	static const struct {
		const char *input;
		const char *decoded;
	} tests[] = {
		{ "giraffe",
			"Z2lyYWZmZQ==" },
		{ "platypus",
			"cGxhdHlwdXM=" },
		{ "ParastratiosphecomyiaStratiosphecomyioides",
			"UGFyYXN0cmF0aW9zcGhlY29teWlhU3RyYXRpb3NwaGVjb215aW9pZGVz" },
	};
	int i;
	enum ast_test_result_state res = AST_TEST_PASS;

	switch (cmd) {
	case TEST_INIT:
		info->name = "base64_test";
		info->category = "main/utils/";
		info->summary = "base64 test";
		info->description = "This test exercises the base64 conversions.";
		return AST_TEST_NOT_RUN;
	case TEST_EXECUTE:
		break;
	}


	for (i = 0; i < ARRAY_LEN(tests); i++) {
		char tmp[64];
		ast_base64encode(tmp, (unsigned char *)tests[i].input, strlen(tests[i].input), sizeof(tmp));
		if (strcasecmp(tmp, tests[i].decoded)) {
			ast_test_status_update(test,
					"input: '%s'  base64 output: '%s'  expected base64 output: '%s'\n",
					tests[i].input, tmp, tests[i].decoded);
			res = AST_TEST_FAIL;
		}

		memset(tmp, 0, sizeof(tmp));
		ast_base64decode((unsigned char *) tmp, tests[i].decoded, (sizeof(tmp) - 1));
		if (strcasecmp(tmp, tests[i].input)) {
			ast_test_status_update(test,
					"base64 input: '%s'  output: '%s'  expected output: '%s'\n",
					tests[i].decoded, tmp, tests[i].input);
			res = AST_TEST_FAIL;
		}
	}

	return res;
}


static int unload_module(void)
{
	AST_TEST_UNREGISTER(uri_encode_decode_test);
	AST_TEST_UNREGISTER(md5_test);
	AST_TEST_UNREGISTER(sha1_test);
	AST_TEST_UNREGISTER(base64_test);
	return 0;
}

static int load_module(void)
{
	AST_TEST_REGISTER(uri_encode_decode_test);
	AST_TEST_REGISTER(md5_test);
	AST_TEST_REGISTER(sha1_test);
	AST_TEST_REGISTER(base64_test);
	return AST_MODULE_LOAD_SUCCESS;
}

AST_MODULE_INFO_STANDARD(ASTERISK_GPL_KEY, "Utils test module");