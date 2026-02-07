#include "HttpClient.hpp"

#include <curl/curl.h>

#include <cctype>
#include <memory>
#include <sstream>

namespace util
{
    namespace http
    {
        namespace
        {

            size_t WriteBody(char *ptr, size_t size, size_t nmemb, void *userdata)
            {
                auto *body = static_cast<std::string *>(userdata);
                const size_t total = size * nmemb;
                body->append(ptr, total);
                return total;
            }

            std::string Trim(const std::string &value)
            {
                size_t start = 0;
                while (start < value.size() && std::isspace(static_cast<unsigned char>(value[start])) != 0)
                {
                    ++start;
                }
                size_t end = value.size();
                while (end > start && std::isspace(static_cast<unsigned char>(value[end - 1])) != 0)
                {
                    --end;
                }
                return value.substr(start, end - start);
            }

            size_t WriteHeader(char *ptr, size_t size, size_t nmemb, void *userdata)
            {
                auto *headers = static_cast<std::map<std::string, std::string> *>(userdata);
                const size_t total = size * nmemb;
                std::string line(ptr, total);

                const auto pos = line.find(':');
                if (pos != std::string::npos)
                {
                    std::string key = Trim(line.substr(0, pos));
                    std::string value = Trim(line.substr(pos + 1));
                    if (!key.empty())
                    {
                        headers->insert({key, value});
                    }
                }

                return total;
            }

            std::string JoinUrl(const std::string &base, const std::string &path)
            {
                if (base.empty())
                {
                    return path;
                }
                if (path.empty())
                {
                    return base;
                }
                if (base.back() == '/' && path.front() == '/')
                {
                    return base + path.substr(1);
                }
                if (base.back() != '/' && path.front() != '/')
                {
                    return base + '/' + path;
                }
                return base + path;
            }

            HttpResponse PerformRequest(const std::string &method,
                                        const std::string &url,
                                        const std::optional<std::string> &json_body)
            {
                HttpResponse response;
                response.request_method = method;
                response.request_url = url;
                if (json_body.has_value())
                {
                    response.request_body = *json_body;
                }

                CURL *curl = curl_easy_init();
                if (!curl)
                {
                    return response;
                }

                std::string body;
                std::map<std::string, std::string> headers;

                curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
                curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method.c_str());
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteBody);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, &body);
                curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, WriteHeader);
                curl_easy_setopt(curl, CURLOPT_HEADERDATA, &headers);

                curl_slist *raw_headers = nullptr;
                raw_headers = curl_slist_append(raw_headers, "Accept: application/json");
                if (method == "POST" || method == "PUT")
                {
                    raw_headers = curl_slist_append(raw_headers, "Content-Type: application/json");
                }
                std::unique_ptr<curl_slist, decltype(&curl_slist_free_all)> header_list(raw_headers,
                                                                                        curl_slist_free_all);
                if (raw_headers)
                {
                    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, raw_headers);
                }

                if ((method == "POST" || method == "PUT") && json_body.has_value())
                {
                    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_body->c_str());
                    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, json_body->size());
                }

                const CURLcode code = curl_easy_perform(curl);
                if (code == CURLE_OK)
                {
                    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response.status_code);
                    response.body = body;
                    response.headers = std::move(headers);
                }

                curl_easy_cleanup(curl);
                return response;
            }

        } // namespace

        HttpClient::HttpClient(std::string base_url) : base_url_(std::move(base_url)) {}

        HttpResponse HttpClient::Get(const std::string &path) const
        {
            return PerformRequest("GET", JoinUrl(base_url_, path), std::nullopt);
        }

        HttpResponse HttpClient::Post(const std::string &path,
                                      const std::optional<std::string> &json_body) const
        {
            return PerformRequest("POST", JoinUrl(base_url_, path), json_body);
        }

        HttpResponse HttpClient::Put(const std::string &path,
                                     const std::optional<std::string> &json_body) const
        {
            return PerformRequest("PUT", JoinUrl(base_url_, path), json_body);
        }

        HttpResponse HttpClient::Delete(const std::string &path) const
        {
            return PerformRequest("DELETE", JoinUrl(base_url_, path), std::nullopt);
        }

    } // namespace http

} // namespace util