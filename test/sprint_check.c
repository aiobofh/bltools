#include <time.h>

#include <tarsio.h>
#include "sprint.h"
#include "sprint_data.h"

#define m tarsio_mock

/*
 * has_date()
 */
test(has_date_shall_call_is_date_with_correct_arguments_return_0_on_fail) {
  /* By default is_date mock returns 0 = fail */
  assert_eq(0, has_date((char*)1234));
  assert_eq(1, m.is_date.call_count);
  assert_eq((char*)1234, m.is_date.args.arg0);
}

test(has_date_shall_return_0_date_does_not_end_with_space) {
  m.is_date.retval = 1;
  assert_eq(0, has_date("yyyy-mm-ddd"));
}

test(has_date_shall_return_1_date_does_end_with_space) {
  m.is_date.retval = 1;
  assert_eq(1, has_date("yyyy-mm-dd "));
}

/*
 * has_dates()
 */
test(has_dates_shall_call_has_date_for_start_date_with_correct_argument) {
  m.has_date.retval = 0;
  (void)has_dates((char*)1234);
  assert_eq((char*)1234, m.has_date.args.arg0);
}

test(has_dates_shall_call_has_date_for_end_date_with_correct_argument) {
  m.has_date.retval = 1;
  (void)has_dates((char*)1234);
  assert_eq((char*)(1234 + strlen("yyyy-mm-dd ")),
            m.has_date.args.arg0);
}

static int has_date_stub_no_start_but_end(const char* str) {
  if ((char*)1234 == str) { /* Test representation of a string pointer */
    return 0;
  }
  return 1;
}

static int has_date_stub_no_end_but_start(const char* str) {
  if ((char*)1234 == str) { /* Test representation of a string pointer */
    return 1;
  }
  return 0;
}

test(has_dates_shall_return_0_if_the_sprint_has_no_start_but_end_date) {
  m.has_date.func = has_date_stub_no_start_but_end;
  assert_eq(0, has_dates((char*)1234));
}

test(has_dates_shall_return_0_if_the_sprint_has_no_end_but_start_date) {
  m.has_date.func = has_date_stub_no_end_but_start;
  assert_eq(0, has_dates((char*)1234));
}

test(has_dates_shall_return_1_if_the_sprint_has_both_start_and_end_dates) {
  m.has_date.retval = 1;
  assert_eq(1, has_dates((char*)1234));
}

/*
 * has_schedule()
 */
test(has_schedule_shall_return_0_if_the_sprint_has_no_schedule_after_dates) {
  m.strlen.func = strlen;
  assert_eq(0, has_schedule("yyyy-mm-dd yyyy-mm-dd")); /* Too short */
  assert_eq(0, has_schedule("yyyy-mm-dd yyyy-mm-dd ")); /* Too short */
  assert_eq(0, has_schedule("yyyy-mm-dd yyyy-mm-dd MoTu")); /* Too short */
  assert_eq(0, has_schedule("yyyy-mm-dd yyyy-mm-dd 12 "));
}

test(has_schedule_shall_return_1_if_the_sprint_has_schedule_after_dates) {
  m.strlen.func = strlen;
  m.is_weekday_short.func = is_weekday_short;
  assert_eq(1, has_schedule("yyyy-mm-dd yyyy-mm-dd MoTuWeThFr "));
  assert_eq(1, has_schedule("yyyy-mm-dd yyyy-mm-dd TuWeFr "));
  assert_eq(1, has_schedule("yyyy-mm-dd yyyy-mm-dd FrMoTuWe "));
}

/*
 * has_commitment()
 */
test(has_commitment_shall_return_0_if_no_commitment_is_found) {
  m.strlen.func = strlen;
  m.skip_schedule_length.func = skip_schedule_length;

  assert_eq(0, has_commitment("yyyy-mm-dd yyyy-mm-dd DdDd ")); /* Too short */
  assert_eq(0, has_commitment("yyyy-mm-dd yyyy-mm-dd DdDd")); /* Too short */
  assert_eq(0, has_commitment("yyyy-mm-dd yyyy-mm-dd Dd 13")); /* Too short */
  assert_eq(0, has_commitment("yyyy-mm-dd yyyy-mm-dd DdDd ID-directly"));
}

test(has_commitment_shall_return_1_if_commitment_is_found) {
  m.strlen.func = strlen;
  m.skip_schedule_length.func = skip_schedule_length;
  assert_eq(1, has_commitment("yyyy-mm-dd yyyy-mm-dd DdDdDdDd 13 "));
}

/*
 * has_id()
 */
test(has_id_shall_return_0_if_no_id_is_found) {
  assert_eq(0, has_id("yyyy-mm-dd yyyy-mm-dd Dd nn")); /* Too short */
  assert_eq(0, has_id("yyyy-mm-dd yyyy-mm-dd Dd nn ")); /* Too short */
}

test(has_id_shall_return_0_if_id_contains_other_than_alpha_numerics) {
  m.strlen.func = strlen;
  m.skip_schedule_length.func = skip_schedule_length;

  assert_eq(0, has_id("yyyy-mm-dd yyyy-mm-dd Dd nn / b c d"));
  assert_eq(0, has_id("yyyy-mm-dd yyyy-mm-dd Dd nn 1?3 123"));
}

test(has_id_shall_return_1_if_id_is_found) {
  m.strlen.func = strlen;
  m.skip_schedule_length.func = skip_schedule_length;
  assert_eq(1, has_id("yyyy-mm-dd yyyy-mm-dd Dd nn Sprint-102_a"));
}

test(has_id_shall_return_1_if_id_is_found_in_front_of_a_title) {
  m.strlen.func = strlen;
  m.skip_schedule_length.func = skip_schedule_length;
  assert_eq(1, has_id("yyyy-mm-dd yyyy-mm-dd Dd nn Sprint-102_a Title A"));
}

/*
 * get_date()
 */
test(get_date_shall_convert_date_correclty) {
  date_t d;
  get_date(&d, "1234-56-78");
  assert_eq(1234, d.year);
  assert_eq(56, d.month);
  assert_eq(78, d.day);
}

/*
 * get_start_date()
 */
test(get_start_shall_call_get_date_correclty) {
  get_start_date((date_t*)1234, (char*)5678);
  assert_eq(1, m.get_date.call_count);
  assert_eq((date_t*)1234, m.get_date.args.arg0);
  assert_eq((char*)5678, m.get_date.args.arg1);
}

/*
 * get_end_date()
 */
test(get_end_date_shall_convert_date_correclty) {
  get_end_date((date_t*)1234, (char*)5678);
  assert_eq(1, m.get_date.call_count);
  assert_eq((date_t*)1234, m.get_date.args.arg0);
  assert_eq((char*)5678 + strlen("yyyy-mm-dd "),
            m.get_date.args.arg1);
}

/*
 * get_days_in_schedule()
 */
test(get_days_in_chedule_shall_return_the_number_of_scheduled_days) {
  m.strlen.func = strlen;
  m.is_weekday_short.func = is_weekday_short;
  assert_eq(1, get_days_in_schedule("yyyy-mm-dd yyyy-mm-dd Mo "));
  assert_eq(5, get_days_in_schedule("yyyy-mm-dd yyyy-mm-dd MoTuWeThFr "));
  assert_eq(2, get_days_in_schedule("yyyy-mm-dd yyyy-mm-dd FrMo "));
  assert_eq(2, get_days_in_schedule("yyyy-mm-dd yyyy-mm-dd SuMo "));
}

/*
 * get_wday()
 */
test(get_wday_shall_return_1_if_Mo) {
  assert_eq(1, get_wday("Mo"));
}

test(get_wday_shall_return_2_if_Tu) {
  assert_eq(2, get_wday("Tu"));
}

test(get_wday_shall_return_3_if_We) {
  assert_eq(3, get_wday("We"));
}

test(get_wday_shall_return_4_if_Th) {
  assert_eq(4, get_wday("Th"));
}

test(get_wday_shall_return_5_if_Fr) {
  assert_eq(5, get_wday("Fr"));
}

test(get_wday_shall_return_6_if_Sa) {
  assert_eq(6, get_wday("Sa"));
}

test(get_wday_shall_return_0_if_Su) {
  assert_eq(0, get_wday("Su"));
}


/*
 * get_schedule()
 */
test(get_schedule_shall_return_a_list_of_dates) {
  /*
   *    February 2017              March
   * Su Mo Tu We Th Fr Sa   Su Mo Tu We Th Fr Sa
   *           1  2  3  4             1  2  3  4
   *  5  6  7  8  9 10 11    5  6  7  8  9 10 11
   * 12 13 14 15 16 17 18   12 13 14 15 16 17 18
   * 19 20 21 22 23 24 25   19 20 21 22 23 24 25
   * 26 27 28               26 27 28 29 30 31
   */
  const int cnt = 7;
  date_t date[cnt];
  date_t start = {2017, 02, 28};

  m.strlen.func = strlen;
  m.date2tm.func = date2tm;
  m.mktime.func = mktime;
  m.gmtime.func = gmtime;
  m.get_wday.func = get_wday;
  m.is_weekday_short.func = is_weekday_short;

  get_schedule(&start, date, "yyyy-mm-dd yyyy-mm-dd TuWeThSaSuMoSu ");

  /* Tuesday */
  assert_eq(2017, date[0].year);
  assert_eq(2, date[0].month);
  assert_eq(28, date[0].day);

  /* Wednesday */
  assert_eq(2017, date[1].year);
  assert_eq(3, date[1].month);
  assert_eq(1, date[1].day);

  /* Thursday */
  assert_eq(2017, date[2].year);
  assert_eq(3, date[2].month);
  assert_eq(2, date[2].day);

  /* Saturday */
  assert_eq(2017, date[3].year);
  assert_eq(3, date[3].month);
  assert_eq(4, date[3].day);

  /* Sunday */
  assert_eq(2017, date[4].year);
  assert_eq(3, date[4].month);
  assert_eq(5, date[4].day);

  /* Monday */
  assert_eq(2017, date[5].year);
  assert_eq(3, date[5].month);
  assert_eq(6, date[5].day);

  /* Sunday */
  assert_eq(2017, date[6].year);
  assert_eq(3, date[6].month);
  assert_eq(12, date[6].day);
}

/*
 * get_commitment()
 */
test(get_commitment_shall_return_the_commitment_as_an_integer) {
  assert_eq(13, get_commitment("yyyy-mm-dd yyyy-mm-dd Dd 13 Sprint-tjosan"));
}

/*
 * get_id_length()
 */
test(get_id_length_shall_return_the_length_of_the_id) {
  assert_eq(11, get_id_length("yyyy-mm-dd yyyy-mm-dd Dd nn Title-title"));
}

test(get_id_length_shall_return_the_length_of_the_id_trimmed_from_ws) {
  assert_eq(11, get_id_length("yyyy-mm-dd yyyy-mm-dd Dd nn Title-title  "));
}

/*
 * get_id()
 */
test(get_id_shall_copy_the_id_from_a_sprint_to_dest) {
  char dest[32];
  m.memcpy.func = memcpy;
  get_id(dest, 17, "yyyy-mm-dd yyyy-mm-dd DdDd nn Title-title-title");
  assert_eq(0, strcmp(dest, "Title-title-title"));
}

/*
 * is_sprint()
 */
test(is_sprint_should_rely_on_helpers_since_they_are_already_tested) {
  m.has_dates.retval = 1;
  m.has_schedule.retval = 1;
  m.has_commitment.retval = 1;
  m.has_id.retval = 1;

  (void)is_sprint((char*)1234);

  /* has_dates() */
  assert_eq(1, m.has_dates.call_count);
  assert_eq((char*)1234, m.has_dates.args.arg0);

  /* has_schedule() */
  assert_eq(1, m.has_schedule.call_count);
  assert_eq((char*)1234, m.has_schedule.args.arg0);

  /* has_commitment() */
  assert_eq(1, m.has_commitment.call_count);
  assert_eq((char*)1234, m.has_commitment.args.arg0);

  /* has_id() */
  assert_eq(1, m.has_id.call_count);
  assert_eq((char*)1234, m.has_id.args.arg0);
}

test(is_sprint_should_return_0_if_has_dates_fails) {
  m.has_dates.retval = 0;
  m.has_schedule.retval = 1;
  m.has_commitment.retval = 1;
  m.has_id.retval = 1;

  assert_eq(0, is_sprint((char*)1234));
}

test(is_sprint_should_return_0_if_has_schedule_fails) {
  m.has_dates.retval = 1;
  m.has_schedule.retval = 0;
  m.has_commitment.retval = 1;
  m.has_id.retval = 1;

  assert_eq(0, is_sprint((char*)1234));
}

test(is_sprint_should_return_0_if_has_commitment_fails) {
  m.has_dates.retval = 1;
  m.has_schedule.retval = 1;
  m.has_commitment.retval = 0;
  m.has_id.retval = 1;

  assert_eq(0, is_sprint((char*)1234));
}

test(is_sprint_should_return_0_if_has_id_fails) {
  m.has_dates.retval = 1;
  m.has_schedule.retval = 1;
  m.has_commitment.retval = 1;
  m.has_id.retval = 0;

  assert_eq(0, is_sprint((char*)1234));
}

/* TODO: Switch to module_test() macro when supported in cutest. */
test(is_sprint_should_assess_sprints_correctly) {
  /* Integrate helpers by not mocking them */
  /* TODO: Remove these when module_test() macro is done in cutest. */
  m.is_date.retval = 1;
  m.strlen.func = strlen;
  m.is_weekday_short.func = is_weekday_short;
  m.skip_schedule_length.func = skip_schedule_length;
  m.has_date.func = has_date;
  m.has_dates.func = has_dates;
  m.has_schedule.func = has_schedule;
  m.has_id.func = has_id;
  m.has_commitment.func = has_commitment;

  /* Top-level kick-the-tires test of is_story() */
  assert_eq(1, is_sprint("2017-01-01 2017-02-02 MoTuWe 13 201701"));

  m.is_date.retval = 0;

  assert_eq(0, is_sprint("However, this is not"));
}

/*
 * init_dates()
 */
test(init_dates_shall_call_get_start_date_correctly) {
  init_dates((date_t*)1234, (date_t*)5678, (char*)8765);
  assert_eq(1, m.get_start_date.call_count);
  assert_eq((date_t*)1234, m.get_start_date.args.arg0);
  assert_eq((char*)8765, m.get_start_date.args.arg1);
}

test(init_dates_shall_call_get_end_date_correctly) {
  init_dates((date_t*)1234, (date_t*)5678, (char*)8765);
  assert_eq(1, m.get_end_date.call_count);
  assert_eq((date_t*)5678, m.get_end_date.args.arg0);
  assert_eq((char*)8765, m.get_end_date.args.arg1);
}

/*
 * init_schedule()
 */
test(init_schedule_shall_call_get_days_in_schedule_correctly) {
  date_t* schedule;
  int cnt;

  m.malloc.retval = (void*)8765; /* Avoid assert */

  init_schedule(&cnt, (date_t*)1234, &schedule, (char*)5678);

  assert_eq(1, m.get_days_in_schedule.call_count);
  assert_eq((char*)5678, m.get_days_in_schedule.args.arg0);
}

test(init_schedule_shall_call_malloc_correctly) {
  date_t* schedule;
  int cnt;

  m.get_days_in_schedule.retval = 4321;
  m.malloc.retval = (void*)8765; /* Avoid assert */

  init_schedule(&cnt, (date_t*)1234, &schedule, (char*)5678);

  assert_eq(1, m.malloc.call_count);
  assert_eq(sizeof(date_t) * 4321, m.malloc.args.arg0);
}

test(init_schedule_shall_call_get_schedule_correctly) {
  date_t* schedule;
  int cnt;

  m.malloc.retval = (void*)8765; /* Avoid assert */

  init_schedule(&cnt, (date_t*)1234, &schedule, (char*)5678);

  assert_eq(1, m.get_schedule.call_count);
  assert_eq((date_t*)1234, m.get_schedule.args.arg0);
  assert_eq((date_t*)8765, m.get_schedule.args.arg1);
  assert_eq((char*)5678, m.get_schedule.args.arg2);
}

/*
 * init_commitment()
 */
test(init_commitment_shall_call_get_commitment_correclty) {
  int commitment;

  m.get_commitment.retval = 1234;

  init_commitment(&commitment, (char*)5678);

  assert_eq(1, m.get_commitment.call_count);
  assert_eq((char*)5678, m.get_commitment.args.arg0);
  assert_eq(1234, commitment);
}

/*
 * init_id()
 */
test(init_id_shall_call_get_id_length_correctly) {
  char* id;

  m.malloc.retval = (void*)4321; /* Avoid assert */

  init_id(&id, (char*)5678);

  assert_eq(1, m.get_id_length.call_count);
  assert_eq((char*)5678, m.get_id_length.args.arg0);
}

test(init_id_shall_call_malloc_correclty) {
  char* id;

  m.get_id_length.retval = 8765;
  m.malloc.retval = (void*)4321; /* Avoid assert */

  init_id(&id, (char*)5678);

  assert_eq(1, m.malloc.call_count);
  assert_eq(8765 + 1 /* '\0' */, m.malloc.args.arg0);
}

test(init_id_shall_call_get_id_correclty) {
  char* id;

  m.get_id_length.retval = 8765;
  m.malloc.retval = (void*)4321; /* Avoid assert */

  init_id(&id, (char*)5678);

  assert_eq(1, m.get_id.call_count);
  assert_eq(id, m.get_id.args.arg0);
  assert_eq(8765, m.get_id.args.arg1);
  assert_eq((char*)5678, m.get_id.args.arg2);
}

/*
 * sprint_init()
 */
test(sprint_init_shall_call_init_dates_correclty) {
  const char *str = "2017-01-01 2017-02-02 MoFr 03 Slogan";
  sprint_t sprint;
  sprint_init(&sprint, str);
  assert_eq(1, m.init_dates.call_count);
  assert_eq(&sprint.start, m.init_dates.args.arg0);
  assert_eq(&sprint.end, m.init_dates.args.arg1);
  assert_eq(str, m.init_dates.args.arg2);
}

test(sprint_init_shall_call_init_schedule_correclty) {
  const char *str = "2017-01-01 2017-02-02 MoFr 03 Slogan";
  sprint_t sprint;
  sprint_init(&sprint, str);
  assert_eq(1, m.init_schedule.call_count);
  assert_eq(&sprint.schedule_cnt, m.init_schedule.args.arg0);
  assert_eq(&sprint.start, m.init_schedule.args.arg1);
  assert_eq(&sprint.schedule, m.init_schedule.args.arg2);
  assert_eq(str, m.init_schedule.args.arg3);
}

test(sprint_init_shall_call_init_commitment_correclty) {
  const char *str = "2017-01-01 2017-02-02 MoFr 03 Slogan";
  sprint_t sprint;
  sprint_init(&sprint, str);
  assert_eq(1, m.init_commitment.call_count);
  assert_eq(&sprint.commitment, m.init_commitment.args.arg0);
  assert_eq(str, m.init_commitment.args.arg1);
}

test(sprint_init_shall_call_init_id_correclty) {
  const char *str = "2017-01-01 2017-02-02 MoFr 03 Slogan";
  sprint_t sprint;
  sprint_init(&sprint, str);
  assert_eq(1, m.init_id.call_count);
  assert_eq(&sprint.id, m.init_id.args.arg0);
  assert_eq(str, m.init_id.args.arg1);
}

/* TODO: Add a kick-the-tires-test for sprint_init without stubbed world */

/*
 * cleanup_id()
 */
test(cleanup_id_shall_free_the_memory_allocated_for_sprint_id_string) {
  char* id = (char*)1234;
  cleanup_id(&id);
  assert_eq(1, m.free.call_count);
  assert_eq((char*)1234, m.free.args.arg0);
}

/*
 * cleanup_schedule()
 */
test(cleanup_shedule_shall_free_the_memory_allocated_for_sprint_schedule) {
  date_t* schedule = (date_t*)1234;
  cleanup_schedule(&schedule);
  assert_eq(1, m.free.call_count);
  assert_eq((date_t*)1234, m.free.args.arg0);
}

/*
 * sprint_cleanup()
 */
test(sprint_cleanup_shall_call_cleanup_id_correctly) {
  sprint_t sprint;
  sprint_cleanup(&sprint);
  assert_eq(1, m.cleanup_id.call_count);
  assert_eq(&sprint.id, m.cleanup_id.args.arg0);
}

test(sprint_cleanup_shall_call_cleanup_schedule_correctly) {
  sprint_t sprint;
  sprint_cleanup(&sprint);
  assert_eq(1, m.cleanup_schedule.call_count);
  assert_eq(&sprint.schedule, m.cleanup_schedule.args.arg0);
}

/*
 * sprints_read()
 */
test(sprints_read_shall_return_1_if_specified_file_could_not_be_opened) {
  assert_eq(1, sprints_read((const char*)1234, NULL));
}

test(sprints_read_shall_return_2_if_specified_file_could_not_be_read) {
  m.fopen.retval = (FILE*)5678;
  assert_eq(2, sprints_read((const char*)1234, NULL));
}

test(sprints_read_shall_return_3_if_found_an_inbalid_sprint_row) {
  m.fopen.retval = (FILE*)5678;
  m.fgets.retval = (char*)8765;
  assert_eq(3, sprints_read((const char*)1234, NULL));
}

const char* fgets_src2 = "Foo";

char* fgets_with_contents(char* s, int size, FILE* stream) {
  (void)(stream = stream);
  (void)(size = size);
  strcpy(s, fgets_src2);
  return s;
}

int feof_third_call_will_return_1_cnt = 0;
int feof_third_call_will_return_1(FILE* stream) {
  (void)(stream = stream);
  return (feof_third_call_will_return_1_cnt++ > 0);
}

test(sprints_read_shall_call_sprint_init_correctly) {
  feof_third_call_will_return_1_cnt = 0;
  m.fopen.retval = (FILE*)5678;
  m.fgets.func = fgets_with_contents;
  m.feof.func = feof_third_call_will_return_1;
  m.is_sprint.retval = 1;
  assert_eq(0, sprints_read((const char*)1234, NULL));
  assert_eq(1, m.sprint_init.call_count);
  assert_eq(1, NULL != m.sprint_init.args.arg0);
  assert_eq(1, 0 == strncmp("Foo", m.sprint_init.args.arg1, 3));
}

int sprint_row_callback_cnt = 0;
int sprint_row_callback(const sprint_t* sprint) {
  (void)(sprint = sprint);
  sprint_row_callback_cnt++;
  return 0;
}

test(sprints_read_shall_call_callback_function_correctly) {
  feof_third_call_will_return_1_cnt = 0;
  sprint_row_callback_cnt = 0;
  m.fopen.retval = (FILE*)5678;
  m.feof.func = feof_third_call_will_return_1;
  m.fgets.retval = (char*)8765;
  m.is_sprint.retval = 1;
  assert_eq(0, sprints_read((const char*)1234, sprint_row_callback));
  assert_eq(1, sprint_row_callback_cnt);
}
