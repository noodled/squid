
/*
 * $Id: HttpRequest.h,v 1.12 2005/09/12 23:28:57 wessels Exp $
 *
 *
 * SQUID Web Proxy Cache          http://www.squid-cache.org/
 * ----------------------------------------------------------
 *
 *  Squid is the result of efforts by numerous individuals from
 *  the Internet community; see the CONTRIBUTORS file for full
 *  details.   Many organizations have provided support for Squid's
 *  development; see the SPONSORS file for full details.  Squid is
 *  Copyrighted (C) 2001 by the Regents of the University of
 *  California; see the COPYRIGHT file for full details.  Squid
 *  incorporates software developed and/or copyrighted by other
 *  sources; see the CREDITS file for full details.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *  
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111, USA.
 *
 */

#ifndef SQUID_HTTPREQUEST_H
#define SQUID_HTTPREQUEST_H

#include "HttpMsg.h"
#include "client_side.h"
#include "HierarchyLogEntry.h"

/*  Http Request */
extern HttpRequest *requestCreate(method_t, protocol_t, const char *urlpath);
extern void requestDestroy(HttpRequest *);
extern HttpRequest *requestLink(HttpRequest *);
extern void requestUnlink(HttpRequest *);
extern int httpRequestParseHeader(HttpRequest * req, const char *parse_start);
extern void httpRequestSwapOut(const HttpRequest * req, StoreEntry * e);
extern void httpRequestPack(const HttpRequest * req, Packer * p);
extern int httpRequestPrefixLen(const HttpRequest * req);
extern int httpRequestHdrAllowed(const HttpHeaderEntry * e, String * strConnection);
extern int httpRequestHdrAllowedByName(http_hdr_type id);
extern void httpRequestHdrCacheInit(HttpRequest * req);


class HttpHdrRange;

class HttpRequest: public HttpMsg
{

public:
    MEMPROXY_CLASS(HttpRequest);
    HttpRequest();

    virtual void reset();

    bool multipartRangeRequest() const;

    method_t method;
    char login[MAX_LOGIN_SZ];
    char host[SQUIDHOSTNAMELEN + 1];
    auth_user_request_t *auth_user_request;
    u_short port;
    String urlpath;
    char *canonical;
    int link_count;		/* free when zero */
    request_flags flags;
    HttpHdrRange *range;
    time_t ims;
    int imslen;
    int max_forwards;
    /* these in_addr's could probably be sockaddr_in's */

    struct IN_ADDR client_addr;

    struct IN_ADDR my_addr;
    unsigned short my_port;
    unsigned short client_port;
    ConnStateData::Pointer body_connection;	/* used by clientReadBody() */
    HierarchyLogEntry hier;
    err_type errType;
    char *peer_login;		/* Configured peer login:password */
    time_t lastmod;		/* Used on refreshes */
    const char *vary_headers;	/* Used when varying entities are detected. Changes how the store key is calculated */
    char *peer_domain;		/* Configured peer forceddomain */
    String tag;			/* Internal tag for this request */
    String extacl_user;		/* User name returned by extacl lookup */
    String extacl_passwd;	/* Password returned by extacl lookup */
    String extacl_log;		/* String to be used for access.log purposes */

public:
    bool parseRequestLine(const char *start, const char *end);

private:
    const char *packableURI(bool full_uri) const;

protected:
    virtual void packFirstLineInto(Packer * p, bool full_uri) const;
    virtual bool sanityCheckStartLine(MemBuf *buf, http_status *error);

public: // should be private
    void clean(); // low-level; treat as private
};

MEMPROXY_CLASS_INLINE(HttpRequest)

#endif /* SQUID_HTTPREQUEST_H */
