# os_sysstats create table sql
# $Id: os_sysstats.sql,v 1.1.1.1 2005/08/27 15:44:49 jpinto Exp $
#
DROP TABLE IF EXISTS os_sysstats;
CREATE TABLE os_sysstats(
  session_id INT NOT NULL,
  t_when INT NOT NULL,
  ru_maxrss BIGINT,        # /* maximum resident set size */
  ru_ixrss BIGINT,         # /* integral shared memory size */
  ru_idrss BIGINT,         # /* integral unshared data size */
  ru_isrss BIGINT,         # /* integral unshared stack size */
  ru_minflt BIGINT,        # /* page reclaims */
  ru_majflt BIGINT,        # /* page faults */
  ru_nswap BIGINT,         # /* swaps */
  ru_inblock BIGINT,       # /* block input operations */
  ru_oublock BIGINT,       # /* block output operations */
  ru_msgsnd BIGINT,        # /* messages sent */
  ru_msgrcv BIGINT,        # /* messages received */
  ru_nsignals BIGINT,      # /* signals received */
  ru_nvcsw BIGINT,         # /* voluntary context switches */
  ru_nivcsw BIGINT        # /* involuntary context switches */
) Type = InnoDB;
