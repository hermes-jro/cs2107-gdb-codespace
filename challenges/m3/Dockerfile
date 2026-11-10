FROM debian:bookworm-slim

RUN apt-get update \
    && apt-get install -y --no-install-recommends socat \
    && rm -rf /var/lib/apt/lists/* \
    && useradd --create-home --uid 10001 challenge

WORKDIR /app
COPY addressed-to-you flag.txt entrypoint.sh ./
RUN chmod 0555 /app/addressed-to-you /app/entrypoint.sh \
    && chmod 0444 /app/flag.txt \
    && chown -R challenge:challenge /app

USER challenge
EXPOSE 5000
ENTRYPOINT ["/app/entrypoint.sh"]
