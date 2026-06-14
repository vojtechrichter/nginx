#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

static ngx_int_t ngx_http_cachekey_init(ngx_conf_t *cf);

static ngx_http_output_header_filter_pt  ngx_http_next_header_filter;

static ngx_int_t
ngx_http_cachekey_filter(ngx_http_request_t *r)
{
#if (NGX_HTTP_CACHE)
	if (r->cache != NULL) {
		u_char *p;
		ngx_table_elt_t *h;

		p = ngx_pnalloc(r->pool, 8);
		if (p == NULL) {
			return NGX_ERROR;
		}

		h = ngx_list_push(&r->headers_out.headers);
		if (h == NULL) {
			return NGX_ERROR;
		}

		h->hash = 1;
		h->next = NULL;
		ngx_str_set(&h->key, "X-Cache-Key");
		h->value.data = p;
		h->value.len = ngx_sprintf(p, "%08xD", r->cache->crc32) - p;
	}
#endif

	return ngx_http_next_header_filter(r);
}

static ngx_int_t
ngx_http_cachekey_init(ngx_conf_t *cf)
{
	ngx_http_next_header_filter = ngx_http_top_header_filter;
	ngx_http_top_header_filter = ngx_http_cachekey_filter;
	return NGX_OK;
}

static ngx_http_module_t ngx_http_cachekey_module_ctx = {
	NULL,
	ngx_http_cachekey_init,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};

ngx_module_t ngx_http_cachekey_module = {
	NGX_MODULE_V1,
	&ngx_http_cachekey_module_ctx,
	NULL,
	NGX_HTTP_MODULE,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NGX_MODULE_V1_PADDING
};
